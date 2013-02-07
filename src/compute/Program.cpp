#include "Context.hpp"
#include "Program.hpp"
#include "Kernel.hpp"

namespace compute{

Program::Program(cl_program program, const Context& context)
:_handle(program), _device(context.getDevice()), _built(false){
}

Program::Program(const Program& other){
    _handle = other._handle;
    _device = other._device;
    _built = other._built;
    //Increase the reference count
    clRetainProgram(_handle);
}

Program& Program::operator =(const Program& other){
    if(this != &other){
        _handle = other._handle;
        _device = other._device;
        _built = other._built;
        //Increase the reference count
        clRetainProgram(_handle);
    }
    return *this;
}

Program::~Program(){
    //Decrease the reference count
    clReleaseProgram(_handle);
}

void Program::build(){
    this->build("");
}

void Program::build(const std::string& options){
    int err = clBuildProgram(_handle, 0, nullptr, options.c_str(), nullptr, nullptr);
    if(err == CL_SUCCESS){
        _built = true;
    }
}

bool Program::isBuilt() const{
    return _built;
}

std::string Program::getBuildLog() const{
    //This buffer must be large enough to contain the full build log
    static const int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    clGetProgramBuildInfo(_handle, _device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, nullptr);
    return std::string(buffer);
}

Kernel Program::getKernel(const std::string& kernelName){
    cl_kernel kernel = clCreateKernel(_handle, kernelName.c_str(), nullptr);
    Kernel res(kernel, _device);
    return res;
}

cl_program Program::getHandle() const{
    return _handle;
}

Program::operator cl_program() const{
    return _handle;
}

}
