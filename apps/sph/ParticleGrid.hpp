#ifndef SPH_PARTICLEGRID_HPP
#define SPH_PARTICLEGRID_HPP

#include "renderer/Buffer.hpp"
#include "renderer/Texture.hpp"

#include "compute/Program.hpp"
#include "compute/Kernel.hpp"
#include "compute/Buffer.hpp"

#include "HistoPyramid.hpp"

namespace compute{
    class Context;
    class CommandQueue;
}

class ParticleGrid{
    public:
        ParticleGrid(compute::Context& context, int gridSize, int particleCount);

        void update(compute::CommandQueue commands, compute::Buffer particles);

        renderer::Texture& getSizeTexture();
        renderer::Texture& getOffsetTexture();
        renderer::Texture& getArrayTexture();

    private:
        int _gridSize;
        int _particleCount;

        renderer::TextureBuffer _gridSizeBuffer;
        renderer::Texture _gridSizeTex;
        compute::Buffer _gridSizeCL;

        renderer::TextureBuffer _gridOffsetBuffer;
        renderer::Texture _gridOffsetTex;
        compute::Buffer _gridOffsetCL;

        renderer::TextureBuffer _gridArrayBuffer;
        renderer::Texture _gridArrayTex;
        compute::Buffer _gridArrayCL;

        compute::Program _program;
        compute::Kernel _zeroKernel;
        compute::Kernel _countKernel;
        compute::Kernel _placeKernel;
        compute::Context& _context;

        HistoPyramid _histo;
};

#endif //SPH_PARTICLEGRID_HPP
