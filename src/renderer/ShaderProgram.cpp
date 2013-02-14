#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace renderer{

ShaderProgram* ShaderProgram::_programInUse = NULL;

ShaderProgram::ShaderProgram(){
    _handle = glCreateProgram();
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &_maxAttribs);
    _boundAttribs = new bool[_maxAttribs];
    for(int i=0; i<_maxAttribs; i++){
        _boundAttribs[i] = false;
    }
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(_handle);
    delete[] _boundAttribs;
}


ShaderProgram& ShaderProgram::attach(Shader& shader){
    glAttachShader(_handle, shader);
    return *this;
}

ShaderProgram& ShaderProgram::detach(Shader& shader){
    glDetachShader(_handle, shader);
    return *this;
}

ShaderProgram& ShaderProgram::bindAttrib(int position, const std::string& attribName){
    glBindAttribLocation(_handle, position, attribName.c_str());
    _boundAttribs[position] = true; //TODO chck for out of bounds
    //Dit que ce vertex attrib est utilisé
    return *this;
}

ShaderProgram& ShaderProgram::link(){
    glLinkProgram(_handle);

    //Checking for link errors
    int linked;
    int infoLogLength;
    char* cInfoLog;
    std::string infoLog;
    glGetProgramiv(_handle, GL_LINK_STATUS, &linked);

    if(!linked){
       glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
       cInfoLog = new char[infoLogLength];
       glGetProgramInfoLog(_handle, infoLogLength, &infoLogLength, cInfoLog);
 
       infoLog = cInfoLog;
       delete[] cInfoLog;

       throw "ShaderProgram Link failed :\n" + infoLog;
    }
    
    return *this;
}

ShaderProgram& ShaderProgram::use(){
    if(_programInUse == this){
        return *this;
    }
    
    if(_programInUse != NULL){
        for(int i=0; i<_maxAttribs; i++){
            if(_programInUse->_boundAttribs[i]){
                glDisableVertexAttribArray(i);
            }
        }
    }
    _programInUse = this;
    glUseProgram(_handle);

    for(int i=0; i<_maxAttribs; i++){
        if(_boundAttribs[i]){
            glEnableVertexAttribArray(i);
        }
    }
    //Active les vertex attrib
    //TODO avoid unnecessary enable/disable
    return *this;
}

ShaderProgram& ShaderProgram::uni(const std::string& uniName, float f){
    GLuint location = glGetUniformLocation(_handle, uniName.c_str());
    glProgramUniform1f(_handle, location, f);
    return *this;
}

ShaderProgram& ShaderProgram::uni(const std::string& uniName, const glm::vec2& v){
    GLuint location = glGetUniformLocation(_handle, uniName.c_str());
    glProgramUniform2fv(_handle, location, 1, glm::value_ptr(v));
    return *this;
}

ShaderProgram& ShaderProgram::uni(const std::string& uniName, const glm::vec3& v){
    GLuint location = glGetUniformLocation(_handle, uniName.c_str());
    glProgramUniform3fv(_handle, location, 1, glm::value_ptr(v));
    return *this;
}

ShaderProgram& ShaderProgram::uni(const std::string& uniName, const glm::vec4& v){
    GLuint location = glGetUniformLocation(_handle, uniName.c_str());
    glProgramUniform4fv(_handle, location, 1, glm::value_ptr(v));
    return *this;
}

ShaderProgram& ShaderProgram::uni(const std::string& uniName, Texture& tex){
    GLuint location = glGetUniformLocation(_handle, uniName.c_str());

    int value;
    int type = this->getUniformType(uniName);
    if(ShaderProgram::isSamplerType(type)){
        value = tex.activate();
    }else{
        value = tex.activateAsImage();
    }

    glProgramUniform1i(_handle, location, tex.activate());
    return *this;
}

bool ShaderProgram::isSamplerType(unsigned int type){
    return type == GL_SAMPLER_1D ||
           type == GL_SAMPLER_2D ||
           type == GL_SAMPLER_3D;
    //TODO make the complete list
}

unsigned int ShaderProgram::getUniformType(const std::string& uniName){
    GLuint uniformIndice;
    char const * cName = uniName.c_str();
    glGetUniformIndices(_handle, 1, &cName, &uniformIndice);

    GLuint type;
    char miniBuf;
    int size;
    glGetActiveUniform(_handle, uniformIndice, 0, nullptr, &size, &type, &miniBuf);

    return type;
}

GLuint ShaderProgram::getHandle(){
    return _handle;
}

ShaderProgram::operator GLuint(){
    return _handle;
}

}
