#include "Platform.hpp"
#include "Device.hpp"

namespace compute{

Device::Device():_handle(0){
}

Device::Device(cl_device_id id):_handle(id){
}

Device::~Device(){
}

std::string Device::getExtensions() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetDeviceInfo(*this, CL_DEVICE_EXTENSIONS, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

int Device::getMaxComputeUnits() const{
    int max = 0;
    clGetDeviceInfo(*this, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max), &max, nullptr);
    return max;
}

std::string Device::getName() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetDeviceInfo(*this, CL_DEVICE_NAME, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Device::getProfile() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetDeviceInfo(*this, CL_DEVICE_PROFILE, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Device::getVendor() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetDeviceInfo(*this, CL_DEVICE_VENDOR, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Device::getVersion() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetDeviceInfo(*this, CL_DEVICE_VERSION, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

cl_device_id Device::getHandle() const{
    return _handle;
}

void Device::setHandle(cl_device_id id){
    _handle = id;
}

Device::operator cl_device_id() const{
    return _handle;
}

std::ostream& operator<<(std::ostream& os, const Device& device){
    os << "Device " << device.getName() << std::endl;
    os << "       Profile: " << device.getProfile() << std::endl;
    os << "       Version: " << device.getVersion() << std::endl;
    os << "       Vendor: " << device.getVendor() << std::endl;
    os << "       Extensions: " << device.getExtensions();
    return os;
}

std::vector<Device> getAllDevices(const Platform& platform, DeviceType type){
    //10 ids is enough to get all the platforms
    static const int MAX_IDS = 10;
    cl_device_id ids[MAX_IDS];
    unsigned int num_devices;
    clGetDeviceIDs(platform, static_cast<unsigned int>(type), MAX_IDS, ids, &num_devices);

    std::vector<Device> res(num_devices);
    for(unsigned int i = 0; i < num_devices; i++){
        res[i].setHandle(ids[i]);
    }

    return res;
}

}
