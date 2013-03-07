#ifndef SPH_PARTICLESIMULATOR_HPP
#define SPH_PARTICLESIMULATOR_HPP

#include "compute/Program.hpp"
#include "compute/Kernel.hpp"
#include "compute/Buffer.hpp"

class ParticleGrid;

namespace compute{
    class Context;
    class CommandQueue;
}

class ParticleSimulator{
    public:
        ParticleSimulator(compute::Context& context, int gridSize, int particleCount);

        void advect(compute::CommandQueue& commands, compute::Buffer& oldParticles, compute::Buffer& newParticles);

    private:
        int _gridSize;
        int _particleCount;

        compute::Program _program;
        compute::Kernel _advectKernel;
        compute::Context& _context;
};

#endif //SPH_PARTICLESIMULATOR_HPP
