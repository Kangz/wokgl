#ifndef RENDERER_FRAMEBUFFER_HPP
#define RENDERER_FRAMEBUFFER_HPP

#include <GL/glew.h>

#include "Texture.hpp"

namespace renderer{

class FrameBuffer{
    public:
        FrameBuffer(int width, int height);

        //Keep track of what is attached to avoid memory leaks

        FrameBuffer& bind();
        FrameBuffer& attach(int target, Texture& tex);
        FrameBuffer& attachColor(int colorNum, Texture& tex);
        FrameBuffer& checkStatus();
        GLuint getHandle();
        
        operator GLuint();
        
        static void bindScreen();

    private:
        GLuint _handle;
        int _width;
        int _height;
};

}

#endif //RENDERER_FRAMEBUFFER_HPP
