#include "renderer/renderer.hpp"
#include "compute/compute.hpp"

#include "particle.h"
#include "ParticleGrid.hpp"

const std::string gridProgramSource = ""
    + PARTICLE_HEADER +
    "kernel void zero(write_only global int* buffer){\n"
    "   buffer[get_global_id(0)] = 0;\n"
    "}\n"
    "\n"
    "\n"
    "kernel void count(read_only global float* particles, volatile global int* count){\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   int x = floor(particles[i + PARTICLE_POS_X] * 32);\n"
    "   int y = floor(particles[i + PARTICLE_POS_Y] * 32);\n"
    "   atomic_add(&count[x + 32 * y], 1);\n"
    "}\n"
    "\n"
    "\n"
    "kernel void place(read_only global float* particles, read_only global int* offset,\n"
    "               volatile global int* counters, write_only global int* array){\n"
    "   int i = get_global_id(0) * PARTICLE_SIZE;\n"
    "   int x = floor(particles[i + PARTICLE_POS_X] * 32);\n"
    "   int y = floor(particles[i + PARTICLE_POS_Y] * 32);\n"
    "   int cell = x + 32 * y;\n"
    "   int number = atomic_add(&counters[cell], 1);\n"
    "   array[offset[cell] + number] = i;\n"
    "}\n"
    "";

//TODO: use glClearBufferData
static float bigBuffer[100000];

ParticleGrid::ParticleGrid(compute::Context& context, int gridSize, int particleCount):
    _gridSize(gridSize),
    _particleCount(particleCount),
    _gridSizeBuffer(renderer::BufferUsage::DynamicDraw, gridSize * gridSize, bigBuffer),
    _gridSizeTex(renderer::TextureFormat::R32I),
    _gridOffsetBuffer(renderer::BufferUsage::DynamicDraw, gridSize * gridSize, bigBuffer),
    _gridOffsetTex(renderer::TextureFormat::R32I),
    _gridArrayBuffer(renderer::BufferUsage::DynamicDraw, particleCount, bigBuffer),
    _gridArrayTex(renderer::TextureFormat::R32I),
    _program(context.programFromSource(gridProgramSource)),
    _context(context),
    _histo(context){

    _gridSizeTex.dataFromBuffer(_gridSizeBuffer);
    _gridOffsetTex.dataFromBuffer(_gridOffsetBuffer);
    _gridArrayTex.dataFromBuffer(_gridArrayBuffer);

    _gridSizeCL = compute::bufferFromGL(_context, _gridSizeBuffer);
    _gridOffsetCL = compute::bufferFromGL(_context, _gridOffsetBuffer);
    _gridArrayCL = compute::bufferFromGL(_context, _gridArrayBuffer);

    _program.build();

    if(!_program.isBuilt()){
        std::cerr << "Error in the ParticleGrid program" << std::endl;
        std::cerr << _program.getBuildLog() << std::endl;
    }

    _zeroKernel = _program.getKernel("zero");
    _countKernel = _program.getKernel("count");
    _placeKernel = _program.getKernel("place");
}

void ParticleGrid::update(compute::CommandQueue commands, compute::Buffer particles){
    commands.acquire(_gridSizeCL).apply();
    commands.acquire(_gridOffsetCL).apply();
    commands.acquire(_gridArrayCL).apply();

    compute::Buffer gridCurrentPos = _context.createBuffer(_gridSizeCL.getFullSize());
    commands.range(_zeroKernel).global(_gridSize * _gridSize).apply(_gridSizeCL);
    commands.range(_zeroKernel).global(_gridSize * _gridSize).apply(gridCurrentPos);

    commands.range(_countKernel).global(_particleCount).apply(particles, _gridSizeCL);

    _histo.compute(commands, _gridSizeCL, _gridSize * _gridSize, _gridOffsetCL);

    commands.range(_placeKernel).global(_particleCount).apply(particles, _gridOffsetCL, gridCurrentPos, _gridArrayCL);

    commands.release(_gridSizeCL).apply();
    commands.release(_gridOffsetCL).apply();
    commands.release(_gridArrayCL).apply();
}

renderer::Texture& ParticleGrid::getSizeTexture(){
    return _gridSizeTex;
}

renderer::Texture& ParticleGrid::getOffsetTexture(){
    return _gridOffsetTex;
}

renderer::Texture& ParticleGrid::getArrayTexture(){
    return _gridArrayTex;
}

