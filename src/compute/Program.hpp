#ifndef COMPUTE_PROGRAM_HPP
#define COMPUTE_PROGRAM_HPP

#include <CL/cl.h>
#include <string>

namespace compute{

class Context;
class Kernel;

//Represents and OpenCL program
class Program{
    public:
        Program(cl_program program, const Context& context);
        //Override the copy and assignement constructors
        //to update the reference count
        Program(const Program& other);
        Program& operator =(const Program& other);
        ~Program();

        //Compiles the program
        void build();
        void build(const std::string& options);

        //True only of the program was successfully compiled
        bool isBuilt() const;
        std::string getBuildLog() const;

        Kernel getKernel(const std::string& kernelName);

        cl_program getHandle() const;

        operator cl_program() const;

    private:
        cl_program _handle;
        cl_device_id _device;
        bool _built;
};

}

#endif //COMPUTE_PROGRAM_HPP
