#include "compute/compute.hpp"

#include "utils.hpp"
#include "particle.hpp"
#include "ParticleGrid.hpp"
#include "ParticleSimulator.hpp"

ParticleSimulator::ParticleSimulator(compute::Context& context, int gridSize, int particleCount)
:_gridSize(gridSize), _particleCount(particleCount),
_program(context.programFromSource(getParticleHeader() + loadFile("apps/sph/gpu/simulation.cl"))),
_context(context){
    _program.build();

    if(!_program.isBuilt()){
        std::cerr << "Error in the simulation program" << std::endl;
        std::cerr << _program.getBuildLog() << std::endl;
    }

    _advectKernel = _program.getKernel("advect");
    _computeAverageWeightKernel = _program.getKernel("computeAverageWeight");
    _computeAccelKernel = _program.getKernel("computeAccel");
    _addSpeedKernel = _program.getKernel("addSpeed");
}


void ParticleSimulator::advect(compute::CommandQueue& commands, compute::Buffer& oldParticles, compute::Buffer& newParticles, float g){
    commands.range(_advectKernel).global(_particleCount).apply(oldParticles, newParticles, g);
}

void ParticleSimulator::computeAverageWeight(compute::CommandQueue& commands, compute::Buffer& particles, ParticleGrid& grid){
    grid.acquireCL(commands);
    commands.range(_computeAverageWeightKernel).global(_particleCount).apply(particles,
                grid.getSizeCL(), grid.getOffsetCL(), grid.getArrayCL(), _gridSize);
    grid.releaseCL(commands);
}

void ParticleSimulator::computeAccel(compute::CommandQueue& commands, compute::Buffer& particles, ParticleGrid& grid){
    grid.acquireCL(commands);
    commands.range(_computeAccelKernel).global(_particleCount).apply(particles,
                grid.getSizeCL(), grid.getOffsetCL(), grid.getArrayCL(), _gridSize);
    grid.releaseCL(commands);
}

void ParticleSimulator::addSpeed(compute::CommandQueue& commands, compute::Buffer& particles, float x, float y, float r, float vx, float vy){
    commands.range(_addSpeedKernel).global(_particleCount).apply(particles, x, y, r, vx, vy);
}

