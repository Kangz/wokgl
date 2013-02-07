#ifndef COMPUTE_PLATFORM_HPP
#define COMPUTE_PLATFORM_HPP

#include <CL/cl.h>
#include <vector>
#include <string>
#include <iostream>

namespace compute{

//Represents and OpenCL platform
class Platform{
    public:
        Platform();
        Platform(cl_platform_id id);
        ~Platform();

        //Getters for information about the device
        std::string getExtensions() const;
        std::string getName() const;
        std::string getProfile() const;
        std::string getVendor() const;
        std::string getVersion() const;

        cl_platform_id getHandle() const;
        void setHandle(cl_platform_id id);

        operator cl_platform_id() const;

    private:
        cl_platform_id _handle;
};

//Allows a platform to be printed
std::ostream& operator<<(std::ostream& os, const Platform& platform);

//Returns the list of all the OpenCL platforms
std::vector<Platform> getAllPlatforms();

}

#endif //COMPUTE_PLATFORM_HPP
