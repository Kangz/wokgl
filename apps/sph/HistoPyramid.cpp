#include "compute/compute.hpp"
#include "HistoPyramid.hpp"

static const std::string histoSource = ""
    "kernel void collect(read_only global int* input, write_only global int* output,\n"
    "                       const int inputLength, const int factor){\n"
    "   int i = get_global_id(0);\n"
    "   if(i == 0){\n"
    "       output[i] = 0;\n"
    "       return;\n"
    "   }\n"
    "   int res = 0;\n"
    "   int start = (i - 1) * factor;\n"
    "   for(int j = 0; j < factor; j++){\n"
    "       if(start + j < inputLength){\n"
    "           res += input[start + j];\n"
    "       }\n"
    "   }\n"
    "   output[i] = res;\n"
    "}\n"
    "\n"
    "\n"
    "kernel void expand(read_only global int* input, read_only global int* collectedInput, write_only global int* output,\n"
    "                       const int inputLength, const int factor){"
    "   int i = get_global_id(0);\n"
    "   int accum = collectedInput[i];\n"
    "   int start = i * factor;\n"
    "   for(int j = 0; j < factor; j++){\n"
    "       if(start + j < inputLength){\n"
    "           output[start + j] = accum;\n"
    "           accum += input[start + j];\n"
    "       }\n"
    "   }\n"
    "}\n"
    "\n"
    "kernel void expandInternal(read_only global int* input, read_only global int* collectedInput, write_only global int* output,\n"
    "                       const int inputLength, const int factor){"
    "   int i = get_global_id(0);\n"
    "   int accum = collectedInput[i];\n"
    "   int start = i * factor;\n"
    "   for(int j = 0; j < factor; j++){\n"
    "       if(start + j < inputLength){\n"
    "           accum += input[start + j];\n"
    "           output[start + j] = accum;\n"
    "       }\n"
    "   }\n"
    "}\n"
    "\n"
    "\n"
    "\n"
    "kernel void histo(read_only global int* input, write_only global int* output, const int length){\n"
    "   int accum = 0;\n"
    "   for(int i = 0; i < length; i++){\n"
    "       output[i] = accum;\n"
    "       accum += input[i];\n"
    "   }\n"
    "}\n"
    "\n"
    "\n"
    "kernel void histoInternal(read_only global int* input, write_only global int* output, const int length){\n"
    "   int accum = 0;\n"
    "   for(int i = 0; i < length; i++){\n"
    "       accum += input[i];\n"
    "       output[i] = accum;\n"
    "   }\n"
    "}\n"
    "";

HistoPyramid::HistoPyramid(compute::Context& context)
:_program(context.programFromSource(histoSource)), _context(context){
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

