#ifndef COMPUTE_CONTEXT_HPP
#define COMPUTE_CONTEXT_HPP

#include <CL/cl.h>
#include "Device.hpp"

namespace compute{

class Program;
class Buffer;

//Represents an OpenGL context
class Context{
    public:
        Context(const Device& device);
        //Override the copy and assignement constructors
        //to update reference count
        Context(const Context& other);
        Context& operator =(const Context& other);
        ~Context();

        //We put ourselves in a simple case where a single device is used
        //so this method gives the associated device
        Device getDevice() const;

        //create a program from the source but does not build it
        Program programFromSource(const std::string& source) const;

        //Return a new buffer on the device
        Buffer createBuffer(int size) const;

        cl_context getHandle() const;

        operator cl_context() const;

    private:
        cl_context _handle;
        Device _device;
};

}

#endif //COMPUTE_CONTEXT_HPP
