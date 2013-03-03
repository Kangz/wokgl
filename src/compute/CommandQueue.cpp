#include "Context.hpp"
#include "Kernel.hpp"
#include "Buffer.hpp"
#include "Commands.hpp"
#include "CommandQueue.hpp"

namespace compute{

CommandQueue::CommandQueue(){
}

CommandQueue::CommandQueue(const Context& context){
    _handle = clCreateCommandQueue(context, context.getDevice(), 0, nullptr);
}

CommandQueue::CommandQueue(const CommandQueue& other){
    _handle = other._handle;
    //Increase the reference count
    clRetainCommandQueue(_handle);
}

CommandQueue& CommandQueue::operator =(const CommandQueue& other){
    if(this != &other){
        _handle = other._handle;
        //Increase the reference count
        clRetainCommandQueue(_handle);
    }
    return *this;
}

CommandQueue::~CommandQueue(){
    //Decrease the reference count
    clReleaseCommandQueue(_handle);
}

UploadCommand CommandQueue::upload(void const * src, Buffer dest){
    return UploadCommand(*this, src, dest);
}

DownloadCommand CommandQueue::download(Buffer src, void* dest){
    return DownloadCommand(*this, src, dest);
}

NDRangeCommand CommandQueue::range(Kernel kernel){
    return NDRangeCommand(*this, kernel);
}

AcquireCommand CommandQueue::acquire(Buffer buffer){
    return AcquireCommand(*this, buffer);
}

ReleaseCommand CommandQueue::release(Buffer buffer){
    return ReleaseCommand(*this, buffer);
}

void CommandQueue::waitFinish(){
    clFinish(_handle);
}

cl_command_queue CommandQueue::getHandle() const{
    return _handle;
}

CommandQueue::operator cl_command_queue() const{
    return _handle;
}

}
