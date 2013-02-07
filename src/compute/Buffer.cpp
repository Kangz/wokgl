#include "Buffer.hpp"

namespace compute{

Buffer::Buffer(cl_mem mem, int size):_handle(mem), _size(size){
}

Buffer::Buffer(const Buffer& other){
    _handle = other._handle;
    _size = other._size;
    //Increase the reference count
    clRetainMemObject(_handle);
}

Buffer& Buffer::operator =(const Buffer& other){
    if(this != &other){
        _handle = other._handle;
        _size = other._size;
        //Increase the reference count
        clRetainMemObject(_handle);
    }
    return *this;
}

Buffer::~Buffer(){
    //Decrease the reference count
    clReleaseMemObject(_handle);
}

cl_mem Buffer::getHandle() const{
    return _handle;
}

int Buffer::getFullSize() const{
    return _size;
}

Buffer::operator cl_mem() const{
    return _handle;
}

}
