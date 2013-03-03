#ifndef SPH_HISTOPYRAMID_HPP
#define SPH_HISTOPYRAMID_HPP

#include "compute/Program.hpp"
#include "compute/Kernel.hpp"

namespace compute{
    class Context;
    class CommandQueue;
}

class HistoPyramid{
    public:
        HistoPyramid(compute::Context& context);
        void compute(compute::CommandQueue& commands, compute::Buffer in, int length, compute::Buffer out);

    private:
        void internalCompute(compute::CommandQueue& commands, compute::Buffer in, int length, compute::Buffer out);

        int _factor;
        compute::Program _program;
        compute::Kernel _collectKernel;
        compute::Kernel _expandKernel;
        compute::Kernel _expandInternalKernel;
        compute::Kernel _histoKernel;
        compute::Kernel _histoInternalKernel;
        compute::Context& _context;
};

#endif //SPH_HISTOPYRAMID_HPP
