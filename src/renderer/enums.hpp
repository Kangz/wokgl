#ifndef RENDERER_ENUMS_HPP
#define RENDERER_ENUMS_HPP

#include <GL/glew.h>

namespace renderer{

//Active manager-related enums

enum class ActiveManagerType{
    Sampler = 0,
    Image = 1,
    Max = 2,
};

//Buffer-related enums

enum class BufferTarget{
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    PixelPack = GL_PIXEL_PACK_BUFFER,
    PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
    Texture = GL_TEXTURE_BUFFER,
};

enum class BufferUsage{
    StreamDraw = GL_STREAM_DRAW,
    StreamRead = GL_STREAM_READ,
    StreamCopy = GL_STREAM_COPY,
    StaticDraw = GL_STATIC_DRAW,
    StaticRead = GL_STATIC_READ,
    StaticCopy = GL_STATIC_COPY,
    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY,
};

//Debug-related enums

enum class DebugType {
    Error = GL_DEBUG_TYPE_ERROR_ARB,
    DeprecatedBehavior = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB,
    UndefinedBehavior = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,
    Portability = GL_DEBUG_TYPE_PORTABILITY_ARB,
    Performance = GL_DEBUG_TYPE_PERFORMANCE_ARB,
    Other = GL_DEBUG_TYPE_OTHER_ARB,
};

enum class DebugSeverity {
    Low = GL_DEBUG_SEVERITY_LOW_ARB,
    Medium = GL_DEBUG_SEVERITY_MEDIUM_ARB,
    High = GL_DEBUG_SEVERITY_HIGH_ARB,
};

enum class DebugSourceAPI {
    OpenGL = GL_DEBUG_SOURCE_API_ARB,
    WindowSystem = GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,
    ShaderCompiler = GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
    ThirdParty = GL_DEBUG_SOURCE_THIRD_PARTY_ARB,
    Application = GL_DEBUG_SOURCE_APPLICATION_ARB,
    Other = GL_DEBUG_SOURCE_OTHER_ARB,
};

//Shader-related enums

enum class ShaderType {
    FragmentShader = GL_FRAGMENT_SHADER,
    VertexShader = GL_VERTEX_SHADER,
};

//Texture-related enums

enum class TextureFilter{
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
};

enum class TextureFormat{
   RGBA8 = GL_RGBA8,

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

enum class TextureTarget{
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    Texture3D = GL_TEXTURE_3D,
    TextureBuffer = GL_TEXTURE_BUFFER,
};

enum class TextureWrap{
    Clamp = GL_CLAMP_TO_EDGE,
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
};

}

#endif //RENDERER_TEXTURE_HPP
