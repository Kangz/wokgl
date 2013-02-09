#ifndef COMPUTE_DEVICE_HPP
#define COMPUTE_DEVICE_HPP

#include <CL/cl.h>
#include <vector>
#include <string>
#include "enums.hpp"

namespace compute{

class Platform;

//Represents an OpenCL device
class Device{
    public:
        Device();
        Device(cl_device_id id);
        ~Device();

        //Getters for information about the device
        std::string getExtensions() const;
        int getMaxComputeUnits() const;
        std::string getName() const;
        std::string getProfile() const;
        std::string getVendor() const;
        std::string getVersion() const;

        cl_platform_id getPlatform() const;
        void setPlatform(cl_platform_id platform);

        cl_device_id getHandle() const;
        void setHandle(cl_device_id id);

        operator cl_device_id() const;

    private:
        cl_device_id _handle;
        cl_platform_id _platform;
};

//Allow a device to be easily printed
std::ostream& operator<<(std::ostream& os, const Device& device);

//Return a vector of all devices for the given platform
std::vector<Device> getAllDevices(const Platform& platform, DeviceType type=DeviceType::GPU);

}

#endif //COMPUTE_DEVICE_HPP
