#ifndef RENDERER_BUFFER_HPP
#define RENDERER_BUFFER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace renderer{

class Buffer{
    public:
        Buffer(int bufferType, int usage);
        Buffer(int bufferType, int usage, int size, const glm::vec2* data);
        ~Buffer();

        Buffer& bind();
        //this depends on glm being storing vectors in the right way
        Buffer& feed(int size, const float* data);
        Buffer& feed(int size, const glm::vec2* data);
        Buffer& feed(int size, const glm::vec3* data);
        Buffer& feed(int size, const glm::vec4* data);

        GLuint getDataType();
        int getDataSize();

        GLuint getHandle();
        
        operator GLuint();

    private:
        void initialise(int bufferType, int usage);
        
        GLuint _handle;
        int _usage;
        int _bufferType;
        GLuint _dataType;
        int _dataSize;
};

}

#endif //RENDERER_BUFFER_HPP
