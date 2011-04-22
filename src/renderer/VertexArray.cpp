#include "VertexArray.hpp"

namespace renderer{

VertexArray::VertexArray(){
    glGenVertexArrays(1, &_handle);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &_handle);
}

VertexArray& VertexArray::bind(){
    glBindVertexArray(_handle);
    return *this;
}

VertexArray& VertexArray::setAttribute(int attribute, Buffer& buf){
    this->bind();
    buf.bind();
    glVertexAttribPointer(attribute, buf.getDataSize(), buf.getDataType(), GL_FALSE, 0, 0);
    return *this;
}

GLuint VertexArray::getHandle(){
    return _handle;
};

VertexArray::operator GLuint(){
    return _handle;
}

}
