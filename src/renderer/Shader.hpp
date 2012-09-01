#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <string>
#include <GL/glew.h>
#include "enums.hpp"

namespace renderer{

class Shader{
    public:
        Shader(ShaderType type);
        Shader(ShaderType type, const std::string& source);
        ~Shader();

        Shader& load(const std::string& source);
        
        GLuint getHandle();
        
        operator GLuint();
        
    private:
        GLuint _handle;
        ShaderType _type;
};

//These are shortcuts and do not deserve their own file
class FragmentShader: public Shader{
    public:
        FragmentShader(): Shader(ShaderType::FragmentShader){}
        FragmentShader(const std::string& source): Shader(ShaderType::FragmentShader, source){}
};

class VertexShader: public Shader{
    public:
        VertexShader(): Shader(ShaderType::VertexShader){}
        VertexShader(const std::string& source): Shader(ShaderType::VertexShader, source){}
};

}

#endif //RENDERER_SHADER_HPP
