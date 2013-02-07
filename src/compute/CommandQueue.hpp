#ifndef COMPUTE_COMMAND_QUEUE_HPP
#define COMPUTE_COMMAND_QUEUE_HPP

#include <CL/cl.h>

namespace compute{

class Context;
class Kernel;
class Buffer;
class UploadCommand;
class DownloadCommand;
class NDRangeCommand;

//Represents an OpenCL command queue
class CommandQueue{
    public:
        CommandQueue(const Context& context);
        //Override copy and assignement constructors
        //tu update the reference count
        CommandQueue(const CommandQueue& other);
        CommandQueue& operator =(const CommandQueue& other);
        ~CommandQueue();

        //These are the actual commands: they return command
        //objects to allow a more declarative use of OpenCL
        
        //Command to move data from the host to the device
        UploadCommand upload(void const * src, Buffer dest);
        //Command to move data from the device to the host
        DownloadCommand download(Buffer src, void* dest);
        //Launch a computation on the device
        NDRangeCommand range(Kernel kernel);

        //Blocks until the queue is empty
        void waitFinish();

        cl_command_queue getHandle() const;

        operator cl_command_queue() const;

    private:
        cl_command_queue _handle;
};

}

#endif //COMPUTE_COMMAND_QUEUE_HPP
