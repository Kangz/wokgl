#ifndef RENDERER_ENUMS_HPP
#define RENDERER_ENUMS_HPP

#include <GL/glew.h>

namespace renderer{

enum class ShaderType {
    FragmentShader = GL_FRAGMENT_SHADER,
    VertexShader = GL_VERTEX_SHADER,
};

enum class TextureFilter {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
};

enum class TextureFormat {
   RGBA = GL_RGBA,

   //Half floats
   R16F = GL_R16F,
   RG16F = GL_RG16F,
   RGB16F = GL_RGB16F,
   RGBA16F = GL_RGBA16F,

   //Floats
   R32F = GL_R32F,
   RG32F = GL_RG32F,
   RGB32F = GL_RGB32F,
   RGBA32F = GL_RGBA32F,
};

enum class TextureWrap {
    Clamp = GL_CLAMP_TO_EDGE,
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
};

}

#endif //RENDERER_TEXTURE_HPP
