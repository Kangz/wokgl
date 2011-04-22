#ifndef RENDERER_VERTEXARRAY_HPP
#define RENDERER_VERTEXARRAY_HPP

#include <GL/glew.h>

#include "Buffer.hpp"

namespace renderer{

class VertexArray{
    public:
        VertexArray();
        ~VertexArray();
        
        VertexArray& bind();
        VertexArray& setAttribute(int attribute, Buffer& buf);

        GLuint getHandle();
        
        operator GLuint();

    private:
        GLuint _handle;    
};

}

#endif //RENDERER_VERTEXARRAY_HPP
