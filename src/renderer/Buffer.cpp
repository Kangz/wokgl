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
    _dataCount = size;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec2* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*2*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 2;
    _dataCount = size;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec3* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*3*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 3;
    _dataCount = size;
    return *this;
}

Buffer& Buffer::feed(int size, const glm::vec4* data){
    this->bind();
    glBufferData(static_cast<int>(_target), size*4*sizeof(GL_FLOAT), data, static_cast<int>(_usage));
    _dataType = GL_FLOAT;
    _dataSize = 4;
    _dataCount = size;
    return *this;
}

GLuint Buffer::getDataType() const{
    return _dataType;
}

int Buffer::getDataSize() const{
    return _dataSize;
}

int Buffer::getDataCount() const{
    return _dataCount;
}

GLuint Buffer::getHandle() const{
    return _handle;
}

Buffer::operator GLuint() const{
    return _handle;
}

}
