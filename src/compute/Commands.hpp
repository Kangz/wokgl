#ifndef COMPUTE_COMMANDS_HPP
#define COMPUTE_COMMANDS_HPP

#include <CL/cl.h>
#include "Kernel.hpp"

namespace compute{

class CommandQueue;
class Buffer;

//A base class for all commands that can be scheduled using events
//TODO: implement the scheduling
class SchedulableCommand{
    public:
        SchedulableCommand& nonBlock();
        virtual void apply() = 0;

    protected:
        SchedulableCommand(CommandQueue queue);

        cl_command_queue _queue;
        bool _blocking;
};

//The command that moves data from the host to the device
class UploadCommand: public SchedulableCommand{
    public:
        UploadCommand(CommandQueue queue, void const * hostBuffer, Buffer buffer);
        
        UploadCommand& srcOffset(int offset);
        UploadCommand& size(int size);
        virtual void apply();

    private:
        void const * _hostBuffer;
        cl_mem _buffer;
        int _offset;
        int _size;
};

//The command that moves data from the device to the host
class DownloadCommand: public SchedulableCommand{
    public:
        DownloadCommand(CommandQueue queue, Buffer buffer, void* hostBuffer);

        DownloadCommand& destOffset(int offset);
        DownloadCommand& size(int size);
        virtual void apply();

    private:
        void* _hostBuffer;
        cl_mem _buffer;
        int _offset;
        int _size;
};

//The command tha launches a parallel computation on the device
class NDRangeCommand: public SchedulableCommand{
    public:
        NDRangeCommand(CommandQueue queue, Kernel kernel);

        //Set the differents dimensions of that computation
        //Take must be taken to always provide the same number
        //of arguments
        NDRangeCommand& global(int sizex);
        NDRangeCommand& global(int sizex, int sizey);
        NDRangeCommand& global(int sizex, int sizey, int sizez);
        NDRangeCommand& globalOffset(int offsetx);
        NDRangeCommand& globalOffset(int offsetx, int offsety);
        NDRangeCommand& globalOffset(int offsetx, int offsety, int offsetz);
        NDRangeCommand& local(int sizex);
        NDRangeCommand& local(int sizex, int sizey);
        NDRangeCommand& local(int sizex, int sizey, int sizez);

        virtual void apply();

        template<typename ... Args> NDRangeCommand& args(const Args& ... args);
        template<typename ... Args> void apply(const Args& ... args);

    private:
        Kernel _kernel;
        int _dim;
        size_t _global[3];
        size_t _globalOffset[3];
        size_t _local[3];
        bool _hasGlobalOffset;
        bool _hasLocal;
};

template<typename ... Args>
NDRangeCommand& NDRangeCommand::args(const Args& ... args){
    _kernel.setArgs(args...);
    return *this;
}

template<typename ... Args>
void NDRangeCommand::apply(const Args& ... args){
    this->args(args...);
    this->apply();
}

}

#endif //COMPUTE_COMMANDS_HPP
