#include <CL/cl_gl.h>
#include <GL/glx.h>

#include "Context.hpp"
#include "Program.hpp"
#include "Buffer.hpp"

namespace compute{

Context::Context(const Device& device){
    _device = device;
    cl_device_id id = _device;
    _handle = clCreateContext(nullptr, 1, &id, nullptr, nullptr, nullptr);
}

Context::Context(const Device& device, cl_context_properties* properties){
    _device = device;
    cl_device_id id = _device;
    _handle = clCreateContext(properties, 1, &id, nullptr, nullptr, nullptr);
}

Context::Context(const Context& other){
    _device = other._device;
    _handle = other._handle;
    //Increase the reference count
    clRetainContext(_handle);
}

Context& Context::operator =(const Context& other){
    if(this != &other){
        _device = other._device;
        _handle = other._handle;
        //Increase the reference count
        clRetainContext(_handle);
    }
    return *this;
}

Device Context::getDevice() const{
    return _device;
}

Context::~Context(){
    //Decrease the reference count
    clReleaseContext(_handle);
}

Program Context::programFromSource(const std::string& source) const{
    const char* csource = source.c_str();
    cl_program program = clCreateProgramWithSource(*this, 1, &csource, nullptr, nullptr);
    Program res(program, *this);
    return res;
}

Buffer Context::createBuffer(int size) const{
    cl_mem mem = clCreateBuffer(_handle, CL_MEM_READ_WRITE, size, nullptr, nullptr);
    Buffer res = Buffer(mem, size);
    return res;
}

cl_context Context::getHandle() const{
    return _handle;
}

Context::operator cl_context() const{
    return _handle;
}

Context getOpenGLInteropContext(const Device& device){
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(glXGetCurrentContext()),
        CL_GLX_DISPLAY_KHR, reinterpret_cast<cl_context_properties>(glXGetCurrentDisplay()),
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(device.getPlatform()),
        0
    };
}

}
