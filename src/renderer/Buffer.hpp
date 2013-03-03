#ifndef RENDERER_BUFFER_HPP
#define RENDERER_BUFFER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "enums.hpp"

namespace renderer{

class Texture;
enum class TextureFormat;

class Buffer{
    public:
        Buffer(BufferTarget target, BufferUsage usage);
        template<typename T> Buffer(BufferTarget target, BufferUsage usage, int size, const T* data);
        ~Buffer();

        Buffer& bind();
        //this depends on glm storing vectors in the right way
        Buffer& feed(int size, const float* data);
        Buffer& feed(int size, const glm::vec2* data);
        Buffer& feed(int size, const glm::vec3* data);
        Buffer& feed(int size, const glm::vec4* data);

        GLuint getDataType() const;
        int getDataSize() const;
        int getDataCount() const;

        GLuint getHandle() const;
        
        operator GLuint() const;

    private:
        GLuint _handle;
        BufferUsage _usage;
        BufferTarget _target;
        GLuint _dataType;
        int _dataSize;
        int _dataCount;
};

class ArrayBuffer: public Buffer{
    public:
        ArrayBuffer(BufferUsage usage): Buffer(BufferTarget::Array, usage){}
        template<typename T> ArrayBuffer(BufferUsage usage, int size, const T* data)
        :Buffer(BufferTarget::Array, usage, size, data){}
};


class ElementBuffer: public Buffer{
    public:
        ElementBuffer(BufferUsage usage): Buffer(BufferTarget::ElementArray, usage){}
        template<typename T> ElementBuffer(BufferUsage usage, int size, const T* data)
        :Buffer(BufferTarget::ElementArray, usage, size, data){}
};

class TextureBuffer: public Buffer{
    public:
        TextureBuffer(BufferUsage usage): Buffer(BufferTarget::Texture, usage){}
        template<typename T> TextureBuffer(BufferUsage usage, int size, const T* data)
        :Buffer(BufferTarget::Texture, usage, size, data){}
};


template<typename T> Buffer::Buffer(BufferTarget target, BufferUsage usage, int size, const T* data): Buffer(target, usage){
    this->feed(size, data);
}

}

#endif //RENDERER_BUFFER_HPP
