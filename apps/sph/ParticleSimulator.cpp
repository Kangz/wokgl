#include "compute/compute.hpp"

#include "particle.h"
#include "ParticleGrid.hpp"
#include "ParticleSimulator.hpp"

const std::string simulationProgramSource = ""
    + PARTICLE_HEADER +
    "#define EPSILON 0.0001f\n"
    "\n"
    "kernel void advect(read_only global float* oldParticles, write_only global float* newParticles){\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   float speedx = oldParticles[i + PARTICLE_SPEED_X];\n"
    "   float speedy = oldParticles[i + PARTICLE_SPEED_Y];\n"
    "   float newposx = oldParticles[i + PARTICLE_POS_X] + speedx;\n"
    "   float newposy = oldParticles[i + PARTICLE_POS_Y] + speedy;\n"
    "\n"
    "   if(newposx < 0.0f + EPSILON){ newposx = 0.0f + EPSILON; speedx = -speedx;}"
    "   if(newposx > 1.0f - EPSILON){ newposx = 1.0f - EPSILON; speedx = -speedx;}"
    "   if(newposy < 0.0f + EPSILON){ newposy = 0.0f + EPSILON; speedy = -speedy;}"
    "   if(newposy > 1.0f - EPSILON){ newposy = 1.0f - EPSILON; speedy = -speedy;}"
    "\n" //TODO do it with a timestep
    "   newParticles[i + PARTICLE_POS_X] = newposx;\n"
    "   newParticles[i + PARTICLE_POS_Y] = newposy;\n"
    "   newParticles[i + PARTICLE_SPEED_X] = speedx;\n"
    "   newParticles[i + PARTICLE_SPEED_Y] = speedy;\n"
    "}\n"
    "";

ParticleSimulator::ParticleSimulator(compute::Context& context, int gridSize, int particleCount)
:_gridSize(gridSize), _particleCount(particleCount),
_program(context.programFromSource(simulationProgramSource)),
_context(context){
    _program.build();

    if(!_program.isBuilt()){
        std::cerr << "Error in the simulation program" << std::endl;
        std::cerr << _program.getBuildLog() << std::endl;
    }

    _advectKernel = _program.getKernel("advect");
}


void ParticleSimulator::advect(compute::CommandQueue& commands, compute::Buffer& oldParticles, compute::Buffer& newParticles){
    commands.range(_advectKernel).global(_particleCount).apply(oldParticles, newParticles);
}


