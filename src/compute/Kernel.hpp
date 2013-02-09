#ifndef COMPUTE_KERNEL_HPP
#define COMPUTE_KERNEL_HPP

#include <CL/cl.h>

namespace compute{

class Program;
class Buffer;

//Represents an OpenCL Kernel
class Kernel{
    public:
        Kernel();
        Kernel(cl_kernel kernel, cl_device_id device);
        //Override copy and assignement constructors to
        //update teh reference count
        Kernel(const Kernel& other);
        Kernel& operator =(const Kernel& other);
        ~Kernel();

        int getWorkGroupSize() const;

        //Functions to set the arguments for this kernel
        //setArgs makes it easy to give all the arguments in one call
        void setArg(int pos, const Buffer& arg);
        template<typename T> void setArg(int pos, const T& arg);
        template<typename ... Args> void setArgs(const Args& ...args);

        cl_kernel getHandle() const;

        operator cl_kernel() const;

    private:
        //Implementation details of the setArgs variadic method template
        template<typename T>
        void setArgs_rec(int pos, const T& arg);
        template<typename T, typename ... Args>
        void setArgs_rec(int pos, const T& arg, const Args& ...args);

        cl_kernel _handle;
        //We are in a simple case with only one device
        cl_device_id _device;
};

template<typename T> void Kernel::setArg(int pos, const T& arg){
    clSetKernelArg(_handle, pos, sizeof(T), &arg);
}

template<typename ... Args> void Kernel::setArgs(const Args& ...args){
    this->setArgs_rec(0, args...);
}

template<typename T>
void Kernel::setArgs_rec(int pos, const T& arg){
    this->setArg(pos, arg);
}

template<typename T, typename ... Args>
void Kernel::setArgs_rec(int pos, const T& arg, const Args& ...args){
    this->setArg(pos, arg);
    this->setArgs_rec(pos + 1, args...);
}

}

#endif //COMPUTE_KERNEL_HPP
