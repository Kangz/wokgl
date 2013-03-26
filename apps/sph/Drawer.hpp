#ifndef SPH_DRAWER_HPP
#define SPH_DRAWER_HPP

#include "renderer/Buffer.hpp"
#include "renderer/ShaderProgram.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VertexArray.hpp"

class ParticleGrid;

namespace renderer{
    class Texture;
}

class Drawer{
    public:
        Drawer(int gridSize, int nParticles);
        void drawDots(ParticleGrid& grid, renderer::Texture& particles);

    private:
        int _gridSize;
        int _nParticles;

        renderer::VertexArray _vao;
        renderer::ArrayBuffer _coordBuffer;
        renderer::VertexShader _vs;

        renderer::FragmentShader _dotsFs;
        renderer::ShaderProgram _dotsProgram;

};

#endif //SPH_DRAWER_HPP
