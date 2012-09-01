#include "FrameBuffer.hpp"

namespace renderer{

FrameBuffer::FrameBuffer(int width, int height): _width(width), _height(height){
    glGenFramebuffers(1, &_handle);
}

FrameBuffer& FrameBuffer::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
    return *this;
}

FrameBuffer& FrameBuffer::attach(int target, Texture& tex){
    this->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, tex, 0);
    return *this;
}

FrameBuffer& FrameBuffer::checkStatus(){ //Thanks r5ge
    this->bind();
    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
        case GL_FRAMEBUFFER_COMPLETE:
            return *this;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw "Some FrameBuffer attachment are incomplete";

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw "This FrameBuffer has no attachement";

        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            throw "The attachments do not all have the same dimension";

        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw "Your hardware does not suport this combination of attachments";

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw "You should call 'glDrawBuffers'"; //WTF is this

        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            throw "Attachment formats are not coherent";

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw "Incomplete read buffer. Did you attach a depth with no color? (Mac issue)"; //TY r5ge here too

        default:
            throw "Unknown FrameBuffer Error";
    }
}

FrameBuffer& FrameBuffer::attachColor(int colorNum, Texture& tex){
    //Check the num is valid
    this->attach(GL_COLOR_ATTACHMENT0 + colorNum, tex);
    return *this;
}

GLuint FrameBuffer::getHandle(){
    return _handle;
}

FrameBuffer::operator GLuint(){
    return _handle;
}

void FrameBuffer::bindScreen(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
