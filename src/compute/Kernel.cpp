#include "Program.hpp"
#include "Buffer.hpp"
#include "Kernel.hpp"

namespace compute{

Kernel::Kernel(cl_kernel kernel, cl_device_id device):_handle(kernel), _device(device){
}

Kernel::Kernel(const Kernel& other){
    _handle = other._handle;
    _device = other._device;
    //Increase the reference count
    clRetainKernel(_handle);
}

Kernel& Kernel::operator =(const Kernel& other){
    if(this != &other){
        _handle = other._handle;
        _device = other._device;
        //Increase the reference count
        clRetainKernel(_handle);
    }
    return *this;
}

Kernel::~Kernel(){
    //Decrease the reference count
    clReleaseKernel(_handle);
}

void Kernel::setArg(int pos, const Buffer& arg){
    this->setArg(pos, arg.getHandle());
}

int Kernel::getWorkGroupSize() const{
    size_t res = 0;
    clGetKernelWorkGroupInfo(_handle, _device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(res), &res, nullptr);
    return res;
}

cl_kernel Kernel::getHandle() const{
    return _handle;
}

Kernel::operator cl_kernel() const{
    return _handle;
}

}
