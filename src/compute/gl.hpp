#ifndef COMPUTE_GL_HPP
#define COMPUTE_GL_HPP

#include <CL/cl.h>

namespace renderer{
    class Buffer;
}

namespace compute{

class Device;
class Context;
class Buffer;

Context getOpenGLInteropContext(const Device& device);

Buffer bufferFromGL(const Context& context, const renderer::Buffer& glBuffer);

}

#endif //COMPUTE_GL_HPP
