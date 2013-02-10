#include <CL/cl_gl.h>

#include "CommandQueue.hpp"
#include "Buffer.hpp"
#include "Commands.hpp"

namespace compute{

SchedulableCommand::SchedulableCommand(CommandQueue queue)
:_queue(queue),_blocking(true){
}

SchedulableCommand& SchedulableCommand::nonBlock(){
    _blocking = false;
    return *this;
}

UploadCommand::UploadCommand(CommandQueue queue, void const * hostBuffer, Buffer buffer)
:SchedulableCommand(queue), _hostBuffer(hostBuffer), _buffer(buffer), _offset(0){
    //By default we upload the full buffer
    _size = buffer.getFullSize();
}

UploadCommand& UploadCommand::srcOffset(int offset){
    _offset = offset;
    return *this;
}

UploadCommand& UploadCommand::size(int size){
    _size = size;
    return *this;
}

void UploadCommand::apply(){
    int err = clEnqueueWriteBuffer(_queue, _buffer, (_blocking ? CL_TRUE: CL_FALSE),
            _offset, _size, _hostBuffer, 0, nullptr, nullptr);
}

DownloadCommand::DownloadCommand(CommandQueue queue, Buffer buffer, void* hostBuffer)
:SchedulableCommand(queue), _hostBuffer(hostBuffer), _buffer(buffer), _offset(0){
    //by default we download the full buffer starting at the beginning at the host pointer
    _size = buffer.getFullSize();
}

DownloadCommand& DownloadCommand::destOffset(int offset){
    _offset = offset;
    return *this;
}

DownloadCommand& DownloadCommand::size(int size){
    _size = size;
    return *this;
}

void DownloadCommand::apply(){
    clEnqueueReadBuffer(_queue, _buffer, (_blocking ? CL_TRUE: CL_FALSE),
            _offset, _size, _hostBuffer, 0, nullptr, nullptr);
}

AcquireCommand::AcquireCommand(CommandQueue queue, Buffer buffer)
:SchedulableCommand(queue), _buffer(buffer){
}

void AcquireCommand::apply(){
    clEnqueueAcquireGLObjects(_queue, 1, &_buffer, 0, nullptr, nullptr);
}

ReleaseCommand::ReleaseCommand(CommandQueue queue, Buffer buffer)
:SchedulableCommand(queue), _buffer(buffer){
}

void ReleaseCommand::apply(){
    clEnqueueReleaseGLObjects(_queue, 1, &_buffer, 0, nullptr, nullptr);
}

NDRangeCommand::NDRangeCommand(CommandQueue queue, Kernel kernel)
:SchedulableCommand(queue), _kernel(kernel), _dim(0), _hasGlobalOffset(false), _hasLocal(false){
}

//TODO add an assert for the dimension
NDRangeCommand& NDRangeCommand::global(int sizex){
    _dim = 1;
    _global[0] = sizex;
    return *this;
}

NDRangeCommand& NDRangeCommand::global(int sizex, int sizey){
    _dim = 2;
    _global[0] = sizex;
    _global[1] = sizey;
    return *this;
}

NDRangeCommand& NDRangeCommand::global(int sizex, int sizey, int sizez){
    _dim = 3;
    _global[0] = sizex;
    _global[1] = sizey;
    _global[2] = sizez;
    return *this;
}

NDRangeCommand& NDRangeCommand::globalOffset(int offsetx){
    _hasGlobalOffset = true;
    _dim = 1;
    _globalOffset[0] = offsetx;
    return *this;
}

NDRangeCommand& NDRangeCommand::globalOffset(int offsetx, int offsety){
    _hasGlobalOffset = true;
    _dim = 2;
    _globalOffset[0] = offsetx;
    _globalOffset[1] = offsety;
    return *this;
}

NDRangeCommand& NDRangeCommand::globalOffset(int offsetx, int offsety, int offsetz){
    _hasGlobalOffset = true;
    _dim = 3;
    _globalOffset[0] = offsetx;
    _globalOffset[1] = offsety;
    _globalOffset[2] = offsetz;
    return *this;
}

NDRangeCommand& NDRangeCommand::local(int sizex){
    _hasLocal = true;
    _dim = 1;
    _local[0] = sizex;
    return *this;
}

NDRangeCommand& NDRangeCommand::local(int sizex, int sizey){
    _hasLocal = true;
    _dim = 2;
    _local[0] = sizex;
    _local[1] = sizey;
    return *this;
}

NDRangeCommand& NDRangeCommand::local(int sizex, int sizey, int sizez){
    _hasLocal = true;
    _dim = 3;
    _local[0] = sizex;
    _local[1] = sizey;
    _local[2] = sizez;
    return *this;
}

void NDRangeCommand::apply(){
    clEnqueueNDRangeKernel(_queue, _kernel, _dim,
            (_hasGlobalOffset ? _globalOffset : nullptr), _global,
            (_hasLocal ? _local : nullptr),
            0, nullptr, nullptr);
}

}
