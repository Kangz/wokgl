#include "compute/compute.hpp"
#include "renderer/renderer.hpp"

#include "utils.hpp"
#include "particle.hpp"
#include "ParticleGrid.hpp"
#include "Drawer.hpp"

const glm::vec2 square[]={
    glm::vec2( 1.0,  1.0),
    glm::vec2( 1.0, -1.0),
    glm::vec2(-1.0,  1.0),
    glm::vec2(-1.0, -1.0),
};

Drawer::Drawer(int gridSize, int nParticles)
:_gridSize(gridSize), _nParticles(nParticles), _vao(), _coordBuffer(renderer::BufferUsage::StaticDraw, 4, square)
{
    const std::string commonHeader = ""
        "#version 430\n"
        "#extension GL_EXT_gpu_shader4 : enable\n"
        + getParticleHeader() + "\n";

    _vao.setAttribute(0, _coordBuffer).bind();

    try{
        _vs.load(commonHeader + loadFile("apps/sph/gpu/commonVert.glsl"));

        _dotsFs.load(commonHeader + loadFile("apps/sph/gpu/dots.glsl"));
        _dotsProgram.attach(_vs).attach(_dotsFs).link().bindAttrib(0, "in_Position");
    }catch(std::string error){
        std::cerr << error << std::endl;
    }catch(const char* error){
        std::cerr << error << std::endl;
    }
}

void Drawer::drawDots(ParticleGrid& grid, renderer::Texture& particles){
    renderer::FrameBuffer::clearColor(0.0, 0.0, 0.0, 0.0);

    _dotsProgram.use().uni("particles", particles)
        .uni("gridSize", grid.getSizeTexture())
        .uni("gridDim", _gridSize)
        .uni("nParticles", _nParticles)
        .uni("gridOffset", grid.getOffsetTexture())
        .uni("gridArray", grid.getArrayTexture());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
