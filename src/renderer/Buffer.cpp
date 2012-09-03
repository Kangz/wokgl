#include "Buffer.hpp"

namespace renderer{

Buffer::Buffer(BufferTarget target, BufferUsage usage){
    _target = target;
    _usage = usage;
    glGenBuffers(1, &_handle);
}

Buffer::~Buffer(){
    glDeleteBuffers(1, &_handle);
}

Buffer& Buffer::bind(){
    glBindBuffer(static_cast<int>(_target), _handle);
    return *this;
}

Buffer& Buffer::feed(int size, const float* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 1;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec2* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*2*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 2;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec3* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*3*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 3;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec4* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*4*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 4;
    return *this;
}

GLuint Buffer::getDataType(){
    return _dataType;
}

int Buffer::getDataSize(){
    return _dataSize;
}

GLuint Buffer::getHandle(){
    return _handle;
}

Buffer::operator GLuint(){
    return _handle;
}

}
