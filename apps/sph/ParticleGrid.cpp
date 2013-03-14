#include "renderer/renderer.hpp"
#include "compute/compute.hpp"

#include "utils.hpp"
#include "particle.hpp"
#include "ParticleGrid.hpp"

//TODO: use glClearBufferData
static float bigBuffer[1000000];

ParticleGrid::ParticleGrid(compute::Context& context, int gridSize, int particleCount):
    _gridSize(gridSize),
    _particleCount(particleCount),
    _gridSizeBuffer(renderer::BufferUsage::DynamicDraw, gridSize * gridSize, bigBuffer),
    _gridSizeTex(renderer::TextureFormat::R32I),
    _gridOffsetBuffer(renderer::BufferUsage::DynamicDraw, gridSize * gridSize, bigBuffer),
    _gridOffsetTex(renderer::TextureFormat::R32I),
    _gridArrayBuffer(renderer::BufferUsage::DynamicDraw, particleCount, bigBuffer),
    _gridArrayTex(renderer::TextureFormat::R32I),
    _program(context.programFromSource(getParticleHeader() + loadFile("apps/sph/gpu/grid.cl"))),
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
    this->acquireCL(commands);

    compute::Buffer gridCurrentPos = _context.createBuffer(_gridSizeCL.getFullSize());
    commands.range(_zeroKernel).global(_gridSize * _gridSize).apply(_gridSizeCL);
    commands.range(_zeroKernel).global(_gridSize * _gridSize).apply(gridCurrentPos);

    commands.range(_countKernel).global(_particleCount).apply(particles, _gridSizeCL, _gridSize);

    _histo.compute(commands, _gridSizeCL, _gridSize * _gridSize, _gridOffsetCL);

    commands.range(_placeKernel).global(_particleCount).apply(particles, _gridOffsetCL, gridCurrentPos, _gridArrayCL, _gridSize);

    this->releaseCL(commands);
}

void ParticleGrid::acquireCL(compute::CommandQueue commands){
    commands.acquire(_gridSizeCL).apply();
    commands.acquire(_gridOffsetCL).apply();
    commands.acquire(_gridArrayCL).apply();
}

void ParticleGrid::releaseCL(compute::CommandQueue commands){
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

compute::Buffer& ParticleGrid::getSizeCL(){
    return _gridSizeCL;
}

compute::Buffer& ParticleGrid::getOffsetCL(){
    return _gridOffsetCL;
}

compute::Buffer& ParticleGrid::getArrayCL(){
    return _gridArrayCL;
}

