#ifndef COMPUTE_BUFFER_HPP
#define COMPUTE_BUFFER_HPP

#include <CL/cl.h>

namespace compute{

//Represents a view of the device memory
//Essentially a wrapper around a cl_mem and a size
class Buffer{
    public:
        Buffer(cl_mem mem, int size);
        //Override copy and assignement constructors to update
        //the cl_mem reference count
        Buffer(const Buffer& other);
        Buffer& operator =(const Buffer& other);
        ~Buffer();

        cl_mem getHandle() const;
        int getFullSize() const;

        operator cl_mem() const;

    private:
        cl_mem _handle;
        int _size;
};

}

#endif //COMPUTE_BUFFER_HPP
