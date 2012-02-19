#ifndef RENDERER_SHADERPROGRAM_HPP
#define RENDERER_SHADERPROGRAM_HPP

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

namespace renderer{

class ShaderProgram{
    public:
        ShaderProgram();
        ~ShaderProgram();

        ShaderProgram& attach(Shader& shader);
        ShaderProgram& detach(Shader& shader);
        ShaderProgram& bindAttrib(int position, const std::string& attribName);
        ShaderProgram& link();
        ShaderProgram& use();
        
        ShaderProgram& uni(const std::string& uniName, float f);         //Mettre tout les string en reference
        ShaderProgram& uni(const std::string& uniName, const glm::vec2& v);        
        ShaderProgram& uni(const std::string& uniName, const glm::vec3& v);        
        ShaderProgram& uni(const std::string& uniName, const glm::vec4& v);
        ShaderProgram& uni(const std::string& uniName, Texture& tex);
        
        GLuint getHandle();
        
        operator GLuint();
                
    private:
        int _maxAttribs; //TODO make it static somehow
        GLuint _handle;
        bool* _boundAttribs;//Replace me with a vector pl0x or bitset
        static ShaderProgram* _programInUse;
};

}

#endif //RENDERER_SHADERPROGRAM_HPP
