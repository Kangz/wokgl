#include <iostream>
#include <random>
#include <string>

#include <SDL.h>
#include <GL/glew.h>

#include "renderer/renderer.hpp"
#include "compute/compute.hpp"

#include "Drawer.hpp"
#include "ParticleSimulator.hpp"
#include "ParticleGrid.hpp"
#include "particle.hpp"

constexpr int SCREEN_SIZE = 800;

constexpr int N_PARTICLES_SQRT = 150;
constexpr int N_PARTICLES = N_PARTICLES_SQRT * N_PARTICLES_SQRT;
constexpr int GRID_SIZE = N_PARTICLES_SQRT * 2;
constexpr float RADIUS = 1.0 / 2 / GRID_SIZE * 3 * 2;
constexpr float INTERACTION_RADIUS = 0.1;

std::mt19937 rng;

Particles* particlesA;
Particles* particlesB;
Particles* currentParticles;
Particles* nextParticles;

compute::Context context;
compute::CommandQueue commands;

void initCL(){
    //Get the platform and device
    auto platforms = compute::getAllPlatforms();
    auto devices = compute::getAllDevices(platforms[0]);
    auto device = devices[0];

    //Create the context
    context = compute::getOpenGLInteropContext(device);
    commands = compute::CommandQueue(context);

    std::cout << platforms[0] << std::endl;
    std::cout << devices[0] << std::endl;
}

float particleData[N_PARTICLES * PARTICLE_FULLSIZE];

void resetParticles(){
    particlesA->bufferGL.feed(N_PARTICLES * PARTICLE_FULLSIZE, particleData);
    particlesB->bufferGL.feed(N_PARTICLES * PARTICLE_FULLSIZE, particleData);
}

void initParticles(){
    //Create the buffer for the particles
    std::uniform_real_distribution<float> distrib(0.0f, 1.0f);

    for(int i = 0; i < PARTICLE_SIZE * N_PARTICLES; i+= PARTICLE_SIZE){
        particleData[i + PARTICLE_POS_X] = distrib(rng) / 2;
        particleData[i + PARTICLE_POS_Y] = distrib(rng) / 1;
        particleData[i + PARTICLE_SPEED_X] = (distrib(rng) - 0.5f) / 50.0f;
        particleData[i + PARTICLE_SPEED_Y] = (distrib(rng) - 0.5f) / 50.0f;
    }

    particlesA = new Particles(context, particleData, N_PARTICLES);
    particlesB = new Particles(context, particleData, N_PARTICLES);
    currentParticles = particlesA;
    nextParticles = particlesB;
}

//Configuration state
float mouseSpeedX = 0.0;
float mouseSpeedY = 0.0;
float mousePosX = 0.0;
float mousePosY = 0.0;
float gravity = true;

bool run(SDL_Window* window){
    rng.seed(0);
    initCL();

    //Set the physical constants
    setMass(0.001f);
    setRadius(RADIUS);
    setCelerity(3.0f);
    setRho0(2.0f);
    setTimestep(0.05f);
    setMu(1);

    initParticles();

    Drawer drawer(GRID_SIZE, N_PARTICLES);
    ParticleSimulator simulator(context, GRID_SIZE, N_PARTICLES);
    ParticleGrid grid(context, GRID_SIZE, N_PARTICLES);

    int lastTicks = SDL_GetTicks();
    bool running = true;
    while(running){
        //Does one step of the simulation
        commands.acquire(currentParticles->bufferCL).apply();
        commands.acquire(nextParticles->bufferCL).apply();
        {
            simulator.advect(commands, currentParticles->bufferCL, nextParticles->bufferCL, gravity ? 4.0 : 0.0);
            grid.update(commands, nextParticles->bufferCL);
            simulator.computeAverageWeight(commands, nextParticles->bufferCL, grid);

            if(mouseSpeedX != 0.0 || mouseSpeedY != 0.0){
                simulator.addSpeed(commands, nextParticles->bufferCL, mousePosX, mousePosY, INTERACTION_RADIUS, mouseSpeedX, mouseSpeedY);

                mouseSpeedX = 0.0;
                mouseSpeedY = 0.0;
            }

            simulator.computeAccel(commands, nextParticles->bufferCL, grid);
        }
        commands.release(currentParticles->bufferCL).apply();
        commands.release(nextParticles->bufferCL).apply();

        //Wait for OpenCL
        commands.waitFinish();

        renderer::FrameBuffer::clearColor(0.0, 0.0, 0.0, 0.0);

        //Draw everything
        drawer.drawDots(grid, nextParticles->tex);
        drawer.drawInteractionRadius(mousePosX, mousePosY, INTERACTION_RADIUS, lastTicks);

        //Wait for OpenGL (SwapWindow has an implicit call to glFinish)
        SDL_GL_SwapWindow(window);

        //Get the time delta to compute the mouse speed
        int currentTicks = SDL_GetTicks();
        int elapsedTicks = currentTicks - lastTicks;
        lastTicks = currentTicks;

        //Handle the events
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_r:
                            resetParticles();
                            break;

                        case SDLK_q:
                            return running = false;
                            break;

                        case SDLK_g:
                            gravity = !gravity;
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    //If there is a drag, compute the mouse speed
                    if(event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        mouseSpeedX = (event.motion.xrel * 1.0 / SCREEN_SIZE) / (elapsedTicks / 1000.0);
                        mouseSpeedY = - (event.motion.yrel * 1.0 / SCREEN_SIZE) / (elapsedTicks / 1000.0);
                    }
                    mousePosX = event.motion.x * 1.0 / SCREEN_SIZE;
                    mousePosY = 1.0 - event.motion.y * 1.0 / SCREEN_SIZE;
                    break;

                case SDL_QUIT:
                    return running = false;
                    break;

                default:
                    break;
            }
        }

        //The new particles are the old particles
        std::swap(currentParticles, nextParticles);
   }

    return true;
}

bool setupWindowAndContext(SDL_Window** window, SDL_GLContext* context){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "Could not initialize SDL" << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    *window = SDL_CreateWindow("Awesome Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr << "Could not initialize the window" << std::endl;
        return false;
    }

    *context = SDL_GL_CreateContext(*window);

    //Initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK){
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }else{
        std::cout << "GLEW init OK" << std::endl;
    }

    //This is Vsync
    SDL_GL_SetSwapInterval(1);
    glEnable(GL_TEXTURE_2D);

    return true;
}

void destroyWindow(SDL_Window* window, SDL_GLContext context){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]){
    SDL_Window* window;
    SDL_GLContext context;

    if(!setupWindowAndContext(&window, &context)){
        return 1;
    }

    renderer::Debug::init();
    renderer::Debug::makeSynchronous();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    if(!run(window)){
        return 1;
    }

    destroyWindow(window, context);
    return 0;
}
