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

        _radiusFs.load(commonHeader + loadFile("apps/sph/gpu/radius.glsl"));
        _radiusProgram.attach(_vs).attach(_radiusFs).link().bindAttrib(0, "in_Position");

        _metaFs.load(commonHeader + loadFile("apps/sph/gpu/meta.glsl"));
        _metaProgram.attach(_vs).attach(_metaFs).link().bindAttrib(0, "in_Position");
    }catch(std::string error){
        std::cerr << error << std::endl;
    }catch(const char* error){
        std::cerr << error << std::endl;
    }
}

void Drawer::drawDots(ParticleGrid& grid, renderer::Texture& particles){
    _dotsProgram.use().uni("particles", particles)
        .uni("gridSize", grid.getSizeTexture())
        .uni("gridDim", _gridSize)
        .uni("nParticles", _nParticles)
        .uni("gridOffset", grid.getOffsetTexture())
        .uni("gridArray", grid.getArrayTexture());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Drawer::drawInteractionRadius(float x, float y, float r, int ticks){
    _radiusProgram.use().uni("x", x).uni("y", y).uni("r", r).uni("time", float(ticks));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Drawer::drawMeta(ParticleGrid& grid, renderer::Texture& particles, float threshold){
    _metaProgram.use().uni("particles", particles)
        .uni("threshold", threshold)
        .uni("gridSize", grid.getSizeTexture())
        .uni("gridDim", _gridSize)
        .uni("nParticles", _nParticles)
        .uni("gridOffset", grid.getOffsetTexture())
        .uni("gridArray", grid.getArrayTexture());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
