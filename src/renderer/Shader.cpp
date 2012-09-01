#include "Shader.hpp"
#include <GL/glew.h>

namespace renderer{

Shader::Shader(ShaderType type): _type(type){
    _handle = glCreateShader(static_cast<int>(_type));
}

Shader::Shader(ShaderType type, const std::string& source): Shader(type){ //TODO: make a typedef for the shadertype
    _handle = glCreateShader(static_cast<int>(_type));
    load(source);
}

Shader::~Shader(){
    glDeleteShader(_handle);
}

Shader& Shader::load(const std::string& source){
    if(source.length() == 0){
        throw "Cannot create a Shader from an empty string";
    }
    const char* cSource = source.c_str();

    glShaderSource(_handle, 1, &cSource, 0);
    glCompileShader(_handle);

    //Checking for compilation errors
    int compiled;
    int infoLogLength;
    char* cInfoLog;
    std::string infoLog;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);
 
    if(!compiled){
       glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
       cInfoLog = new char[infoLogLength];
       glGetShaderInfoLog(_handle, infoLogLength, &infoLogLength, cInfoLog);
 
       infoLog = cInfoLog;
       delete[] cInfoLog;

       throw "Shader Compilation failed :\n" + infoLog;
    }

    return *this;
}

GLuint Shader::getHandle(){
    return _handle;
}

Shader::operator GLuint(){
    return _handle;
}

}
