#include "compute/compute.hpp"
#include "utils.hpp"
#include "HistoPyramid.hpp"

HistoPyramid::HistoPyramid(compute::Context& context)
:_program(context.programFromSource(loadFile("apps/sph/gpu/histo.cl"))), _context(context){
    _program.build();

    if(!_program.isBuilt()){
        std::cerr << "Error in the HistoPyramid program" << std::endl;
        std::cerr << _program.getBuildLog() << std::endl;
    }

    _collectKernel = _program.getKernel("collect");
    _expandKernel = _program.getKernel("expand");
    _expandInternalKernel = _program.getKernel("expandInternal");
    _histoKernel = _program.getKernel("histo");
    _histoInternalKernel = _program.getKernel("histoInternal");

    _factor = context.getDevice().getMaxComputeUnits();
}

void HistoPyramid::compute(compute::CommandQueue& commands, compute::Buffer in, int length, compute::Buffer out){
    if(length <= _factor){
        commands.range(_histoKernel).global(1).apply(in, out, length);
    }else{
        int levelLength = (length + _factor - 1) / _factor;
        compute::Buffer collected = _context.createBuffer(sizeof(int) * levelLength);
        compute::Buffer collectedHisto = _context.createBuffer(sizeof(int) * levelLength);

        commands.range(_collectKernel).global(levelLength).apply(in, collected, length, _factor);
        this->internalCompute(commands, collected, levelLength, collectedHisto);

        commands.range(_expandKernel).global(levelLength).apply(in, collectedHisto, out, length, _factor);
    }
}

void HistoPyramid::internalCompute(compute::CommandQueue& commands, compute::Buffer in, int length, compute::Buffer out){
    if(length <= _factor){
        commands.range(_histoInternalKernel).global(1).apply(in, out, length);
    }else{
        int levelLength = (length + _factor - 1) / _factor;
        compute::Buffer collected = _context.createBuffer(sizeof(int) * levelLength);
        compute::Buffer collectedHisto = _context.createBuffer(sizeof(int) * levelLength);

        commands.range(_collectKernel).global(levelLength).apply(in, collected, length, _factor);
        this->internalCompute(commands, collected, levelLength, collectedHisto);

        commands.range(_expandInternalKernel).global(levelLength).apply(in, collectedHisto, out, length, _factor);
    }
}

