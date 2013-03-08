#include "compute/compute.hpp"

#include "particle.h"
#include "ParticleGrid.hpp"
#include "ParticleSimulator.hpp"

const std::string simulationProgramSource = ""
    + PARTICLE_HEADER +
    "#define EPSILON 0.0001f\n"
    "#define FORGRID(posx, posy, radius, particle_result)\\\n"
    "   int startx = max(0, (int)floor(((posx) - radius) * gridDim));\\\n"
    "   int starty = max(0, (int)floor(((posy) - radius) * gridDim));\\\n"
    "   int endx = min(gridDim, (int)ceil(((posx) + radius) * gridDim));\\\n"
    "   int endy = min(gridDim, (int)ceil(((posy) + radius) * gridDim));\\\n"
    "   for(int x = startx; x < endx; x++){\\\n"
    "       for(int y = starty; y < endy; y++){\\\n"
    "           int cell = x + gridDim * y;\\\n"
    "           int size = gridSize[cell];\\\n"
    "           int offset = gridOffset[cell];\\\n"
    "           for(int l = 0; l < size; l++){\\\n"
    "               (particle_result) = gridArray[offset + l];\n"
    "\n"
    "#define ENDFORGRID }}}"
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
    "\n"
    "float W(float dist, float h){\n"
    "   const float x = dist / h;\n"
    "   if(x < 0.5f){\n"
    "       return 1 + 6 * x * x * (x - 1);\n"
    "   }else if(x < 1.0f){\n"
    "       const float y = 1 - x;\n"
    "       return 2 * y * y * y;\n"
    "   }else{\n"
    "       return 0;\n"
    "   }\n"
    "}\n"
    "\n"
    "kernel void computeAverageWeight(global float* particles, read_only global int* gridSize,\n"
    "       read_only global int* gridOffset, read_only global int* gridArray, int gridDim, float radius){\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   float posx = particles[i + PARTICLE_POS_X];\n"
    "   float posy = particles[i + PARTICLE_POS_Y];\n"
    "   float weight = 0.0f;\n"
    "   int index;\n"
    "   FORGRID(posx, posy, radius, index){\n"
    "       float dx = posx - particles[index + PARTICLE_POS_X];\n"
    "       float dy = posy - particles[index + PARTICLE_POS_Y];\n"
    "       float dist = sqrt(dx * dx + dy * dy);\n"
    "       weight += W(dist, radius);\n"
    "   }ENDFORGRID\n"
    "   particles[i + PARTICLE_AVERAGE_WEIGHT] = weight;\n"
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
    _computeAverageWeightKernel = _program.getKernel("computeAverageWeight");
}


void ParticleSimulator::advect(compute::CommandQueue& commands, compute::Buffer& oldParticles, compute::Buffer& newParticles){
    commands.range(_advectKernel).global(_particleCount).apply(oldParticles, newParticles);
}

void ParticleSimulator::computeAverageWeight(compute::CommandQueue& commands, compute::Buffer& particles, ParticleGrid& grid){
    grid.acquireCL(commands);
    commands.range(_computeAverageWeightKernel).global(_particleCount).apply(particles,
                grid.getSizeCL(), grid.getOffsetCL(), grid.getArrayCL(), _gridSize, 0.04f);
    grid.releaseCL(commands);
}

