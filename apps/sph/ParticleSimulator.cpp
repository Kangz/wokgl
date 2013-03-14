#include "compute/compute.hpp"

#include "particle.h"
#include "ParticleGrid.hpp"
#include "ParticleSimulator.hpp"

const std::string simulationProgramSource = ""
    + PARTICLE_HEADER +
    "#define EPSILON 0.0001f\n"
    "#define FORGRID(posx, posy, radius, particle_result)\\\n"
    "   int startx = max(0, (int)floor(((posx) - radius) * gridDim) - 1000);\\\n"
    "   int starty = max(0, (int)floor(((posy) - radius) * gridDim) - 1000);\\\n"
    "   int endx = min(gridDim, (int)ceil(((posx) + radius) * gridDim) + 1000);\\\n"
    "   int endy = min(gridDim, (int)ceil(((posy) + radius) * gridDim) + 1000);\\\n"
    "   for(int x = startx; x < endx; x++){\\\n"
    "       for(int y = starty; y < endy; y++){\\\n"
    "           int cell = x + gridDim * y;\\\n"
    "           int size = gridSize[cell];\\\n"
    "           int offset = gridOffset[cell];\\\n"
    "           for(int l = 0; l < size; l++){\\\n"
    "               int particle_index = gridArray[offset + l];\\\n"
    "               if(particle_index != get_global_id(0)){\\\n"
    "                   (particle_result) = particle_index;\\\n"
    "\n"
    "#define ENDFORGRID }}}}"
    "\n"
    "kernel void advect(read_only global float* oldParticles, write_only global float* newParticles, const float mass){\n"
    "   float timestep = 0.03f;\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   float accelx = oldParticles[i + PARTICLE_ACCEL_X];\n"
    "   float accely = oldParticles[i + PARTICLE_ACCEL_Y];\n"
    "   float speedx = oldParticles[i + PARTICLE_SPEED_X] + accelx * mass * timestep;\n"
    "   float speedy = oldParticles[i + PARTICLE_SPEED_Y] + accely * mass * timestep - 0.0 * timestep / 100;\n"
    "   float newposx = oldParticles[i + PARTICLE_POS_X] + speedx * timestep;\n"
    "   float newposy = oldParticles[i + PARTICLE_POS_Y] + speedy * timestep;\n"
    "\n"
/*    "   if(newposx < 0.0f + EPSILON){ newposx = 0.0f + EPSILON; speedx = + EPSILON;}"
    "   if(newposx > 1.0f - EPSILON){ newposx = 1.0f - EPSILON; speedx = - EPSILON;}"
    "   if(newposy < 0.0f + EPSILON){ newposy = 0.0f + EPSILON; speedy = + EPSILON;}"
    "   if(newposy > 1.0f - EPSILON){ newposy = 1.0f - EPSILON; speedy = - EPSILON;}"*/
    "   if(newposx < 0.0f + EPSILON){ newposx = 0.0f + EPSILON; speedx = - speedx / 2;}"
    "   if(newposx > 1.0f - EPSILON){ newposx = 1.0f - EPSILON; speedx = - speedx / 2;}"
    "   if(newposy < 0.0f + EPSILON){ newposy = 0.0f + EPSILON; speedy = - speedy / 2 ;}"
    "   if(newposy > 1.0f - EPSILON){ newposy = 1.0f - EPSILON; speedy = - speedy / 2;}"
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
    "float dW(float dist, float h){\n"
    "   const float x = dist / h;\n"
    "   if(x < 0.5f){\n"
    "       return (18 * x - 12) * x / h;\n"
    "   }else if(x < 1.0f){\n"
    "       const float y = 1 - x;\n"
    "       return - 4 / h * y * y;\n"
    "   }else{\n"
    "       return 0;\n"
    "   }\n"
    "}\n"
    "\n"
    "kernel void computeAverageWeight(global float* particles, read_only global int* gridSize,\n"
    "       read_only global int* gridOffset, read_only global int* gridArray, int gridDim, float radius,\n"
    "       const float c, const float weight0){\n"
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
    "   particles[i + PARTICLE_PRESSURE] = c * c * (weight - weight0);\n"
    "}\n"
    "\n"
    "kernel void computeAccel(global float* particles, read_only global int* gridSize,\n"
    "       read_only global int* gridOffset, read_only global int* gridArray, int gridDim, float radius){\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   float posx = particles[i + PARTICLE_POS_X];\n"
    "   float posy = particles[i + PARTICLE_POS_Y];\n"
    "   float own_w = particles[i + PARTICLE_AVERAGE_WEIGHT];\n"
    "   float own_p = particles[i + PARTICLE_PRESSURE];\n"
    "   int index;\n"
    "   float accelx = 0.0f;\n"
    "   float accely = 0.0f;\n"
    "   FORGRID(posx, posy, radius, index){\n"
    "       float dx = posx - particles[index + PARTICLE_POS_X];\n"
    "       float dy = posy - particles[index + PARTICLE_POS_Y];\n"
    "       float other_w = particles[index + PARTICLE_AVERAGE_WEIGHT];\n"
    "       float other_p = particles[index + PARTICLE_PRESSURE];\n"
    "       float dist = sqrt(dx * dx + dy * dy);\n"
    "       float factor = - (own_p / own_w / own_w + other_p / other_w / other_w) / max(0.00000001, dist) * dW(dist, radius);\n"
    "       accelx += dx * factor;\n"
    "       accely += dy * factor;\n"
    "   }ENDFORGRID\n"
    "   particles[i + PARTICLE_ACCEL_X] = accelx;\n"
    "   particles[i + PARTICLE_ACCEL_Y] = accely;\n"
    "}\n"
    "";

ParticleSimulator::ParticleSimulator(compute::Context& context, int gridSize, int particleCount, float radius)
:_gridSize(gridSize), _particleCount(particleCount), _radius(radius),
_program(context.programFromSource(simulationProgramSource)),
_context(context){
    _program.build();

    if(!_program.isBuilt()){
        std::cerr << "Error in the simulation program" << std::endl;
        std::cerr << _program.getBuildLog() << std::endl;
    }

    _advectKernel = _program.getKernel("advect");
    _computeAverageWeightKernel = _program.getKernel("computeAverageWeight");
    _computeAccelKernel = _program.getKernel("computeAccel");
}


void ParticleSimulator::advect(compute::CommandQueue& commands, compute::Buffer& oldParticles, compute::Buffer& newParticles){
    commands.range(_advectKernel).global(_particleCount).apply(oldParticles, newParticles, 0.001f);
}

void ParticleSimulator::computeAverageWeight(compute::CommandQueue& commands, compute::Buffer& particles, ParticleGrid& grid){
    grid.acquireCL(commands);
    commands.range(_computeAverageWeightKernel).global(_particleCount).apply(particles,
                grid.getSizeCL(), grid.getOffsetCL(), grid.getArrayCL(), _gridSize, _radius, 3.0f, 2.0f);
    grid.releaseCL(commands);
}

void ParticleSimulator::computeAccel(compute::CommandQueue& commands, compute::Buffer& particles, ParticleGrid& grid){
    grid.acquireCL(commands);
    commands.range(_computeAccelKernel).global(_particleCount).apply(particles,
                grid.getSizeCL(), grid.getOffsetCL(), grid.getArrayCL(), _gridSize, _radius);
    grid.releaseCL(commands);
}

