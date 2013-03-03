#include <GL/glew.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#include "Device.hpp"
#include "Context.hpp"
#include "Buffer.hpp"
#include "../renderer/Buffer.hpp"
#include "gl.hpp"

namespace compute{

Context getOpenGLInteropContext(const Device& device){
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(glXGetCurrentContext()),
        CL_GLX_DISPLAY_KHR, reinterpret_cast<cl_context_properties>(glXGetCurrentDisplay()),
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(device.getPlatform()),
        0
    };

    return Context(device, properties);
}

Buffer bufferFromGL(const Context& context, const renderer::Buffer& glBuffer){
    cl_mem res =  clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, glBuffer, nullptr);
    return Buffer(res, glBuffer.getDataSize() * glBuffer.getDataCount() * 4);
}

}
