#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <string>
#include <GL/glew.h>

namespace renderer{

class Shader{
    public:
        Shader(int type);
        Shader(int type, const std::string& source);
        ~Shader();

        Shader& load(const std::string& source);
        
        GLuint getHandle();
        
        operator GLuint();
        
    private:
        GLuint _handle;
        int _type;
};

//These are shortcuts and do not deserve their own file
class FragmentShader: public Shader{
    public:
        FragmentShader(): Shader(GL_FRAGMENT_SHADER){}
        FragmentShader(const std::string& source): Shader(GL_FRAGMENT_SHADER, source){}        
};

class VertexShader: public Shader{
    public:
        VertexShader(): Shader(GL_VERTEX_SHADER){}
        VertexShader(const std::string& source): Shader(GL_VERTEX_SHADER, source){}
};

}

#endif //RENDERER_SHADER_HPP
