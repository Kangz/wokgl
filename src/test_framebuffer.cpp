#include <string>
#include <iostream>
#include <fstream>

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "renderer/Buffer.hpp"
#include "renderer/Debug.hpp"
#include "renderer/FrameBuffer.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Shader.hpp"
#include "renderer/ShaderProgram.hpp"
#include "renderer/VertexArray.hpp"

using namespace std;
using namespace renderer;


string loadFile(string filename){
    ifstream in(filename.c_str());

    string str, result;
    getline(in, str);
    while(in){
        result += str + "\n";
        getline(in,str);
    }
    return result + str;
}

void run(SDL_Window* window){
    const glm::vec2 square[4]={
        glm::vec2( 1.0,  1.0),
        glm::vec2( 1.0, -1.0),
        glm::vec2(-1.0,  1.0),
        glm::vec2(-1.0, -1.0)
    };
    
    VertexArray vao;
    ArrayBuffer buf(BufferUsage::StaticDraw, 4, square);
    vao.setAttribute(0, buf).bind();

    ShaderProgram program, invertProgram;
    VertexShader vs, invertVs;
    FragmentShader fs, invertFs;
    Texture tex, tex_normal, rtt;
    
    FrameBuffer fbo(512, 512);

    try{
        vs.load(loadFile("shaders/test_normalmapping.vert"));
        fs.load(loadFile("shaders/test_normalmapping.frag"));
        program.attach(vs).attach(fs).link().bindAttrib(0, "in_Position").use();

        invertVs.load(loadFile("shaders/test_framebuffer.vert"));
        invertFs.load(loadFile("shaders/test_framebuffer.frag"));
        invertProgram.attach(invertVs).attach(invertFs).link().bindAttrib(0, "in_Position").use();

        tex.quickFileLoad("data/floor2c.bmp");
        tex_normal.quickFileLoad("data/floor2c_nm.bmp");
        rtt.emptyData(512, 512);

        fbo.attachColor(0, rtt);
        
        fbo.checkStatus();
    }catch(string error){
        cout << error << "\n" << endl;
    }catch(char const* error){
        cout << error << "\n" << endl;        
    }

    for(int i=0; i<60; i++){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::vec3 lightpos(0.0, 1.0 - i/30.0, 0.3);
        program.use()
               .uni("tex", tex)
               .uni("tex_normal", tex_normal)
               .uni("light", lightpos);

        fbo.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        invertProgram.use()
                     .uni("rtt", rtt);
        
        FrameBuffer::bindScreen();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        SDL_GL_SwapWindow(window);
        SDL_Delay(60);
    }
}



void die(const string msg){
    cout << msg << "\n" << endl;
    exit(1);
}


//This is framework
void setupWindow(SDL_Window** window, SDL_GLContext* context){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        die("Unable to initialize SDL");
    }
 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    *window = SDL_CreateWindow("Awesome Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window){
        die("Unable to create window");
    }

    *context = SDL_GL_CreateContext(*window);
    if (glewInit() != GLEW_OK){
        die("Unable to initialize GLEW");
    }
 
    //This is Vsync
    SDL_GL_SetSwapInterval(1);
    glEnable(GL_TEXTURE_2D);
}

void destroyWindow(SDL_Window* window, SDL_GLContext context){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]){
    SDL_Window* window;
    SDL_GLContext context;
    setupWindow(&window, &context);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK){
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }else{
        std::cout << "GLEW init OK" << std::endl;
    }
    Debug::init();
    Debug::makeSynchronous();

    run(window);
 
    destroyWindow(window, context);
    return 0;
}
