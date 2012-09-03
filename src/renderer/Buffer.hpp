#ifndef RENDERER_BUFFER_HPP
#define RENDERER_BUFFER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace renderer{

class Buffer{
    public:
        Buffer(int bufferType, int usage);
        template<typename T> Buffer(int bufferType, int usage, int size, const T* data);
        ~Buffer();

        Buffer& bind();
        //this depends on glm storing vectors in the right way
        Buffer& feed(int size, const float* data);
        Buffer& feed(int size, const glm::vec2* data);
        Buffer& feed(int size, const glm::vec3* data);
        Buffer& feed(int size, const glm::vec4* data);

        GLuint getDataType();
        int getDataSize();

        GLuint getHandle();
        
        operator GLuint();

    private:
        GLuint _handle;
        int _usage;
        int _bufferType;
        GLuint _dataType;
        int _dataSize;
};

class ArrayBuffer: public Buffer{
    public:
        ArrayBuffer(int usage): Buffer(GL_ARRAY_BUFFER, usage){}
        template<typename T> ArrayBuffer(int usage, int size, const T* data)
        :Buffer(GL_ARRAY_BUFFER, usage, size, data){}
};

template<typename T> Buffer::Buffer(int bufferType, int usage, int size, const T* data): Buffer(bufferType, usage){
    this->feed(size, data);
}

}

#endif //RENDERER_BUFFER_HPP
