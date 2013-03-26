#ifndef SPH_PARTICLE_H
#define SPH_PARTICLE_H

#include <string>
#include "compute/Buffer.hpp"
#include "compute/Context.hpp"
#include "compute/gl.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/Texture.hpp"

#define PARTICLE_SIZE 10
#define PARTICLE_FULLSIZE (PARTICLE_SIZE * sizeof(float))

#define PARTICLE_POS 0
#define PARTICLE_POS_X 0
#define PARTICLE_POS_Y 1
#define PARTICLE_SPEED 2
#define PARTICLE_SPEED_X 2
#define PARTICLE_SPEED_Y 3
#define PARTICLE_AVERAGE_WEIGHT 4
#define PARTICLE_PRESSURE 5
#define PARTICLE_ACCEL_X 6
#define PARTICLE_ACCEL_Y 7
#define PARTICLE_FORCE_X 8
#define PARTICLE_FORCE_Y 9

struct Particles{
    compute::Buffer bufferCL;
    renderer::TextureBuffer bufferGL;
    renderer::Texture tex;

    Particles(compute::Context context, float* data, int nParticles)
    :bufferGL(renderer::BufferUsage::StaticDraw, nParticles * PARTICLE_FULLSIZE, data),
    tex(renderer::TextureFormat::R32F){
        this->tex.dataFromBuffer(this->bufferGL);
        this->bufferCL = compute::bufferFromGL(context, this->bufferGL);
    }
};

std::string getParticleHeader();

void setMass(float mass);
void setRadius(float radius);
void setCelerity(float celerity);
void setRho0(float rho0);
void setTimestep(float timestep);
void setMu(float mu);

#endif //SPH_PARTICLE_H
