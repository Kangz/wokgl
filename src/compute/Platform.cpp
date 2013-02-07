#include <iostream>

#include "Platform.hpp"

namespace compute{

Platform::Platform():_handle(0){
}

Platform::Platform(cl_platform_id id):_handle(id){
}

Platform::~Platform(){
}

std::string Platform::getExtensions() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetPlatformInfo(*this, CL_PLATFORM_EXTENSIONS, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Platform::getName() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetPlatformInfo(*this, CL_PLATFORM_NAME, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Platform::getProfile() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetPlatformInfo(*this, CL_PLATFORM_PROFILE, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Platform::getVendor() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetPlatformInfo(*this, CL_PLATFORM_VENDOR, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

std::string Platform::getVersion() const{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    clGetPlatformInfo(*this, CL_PLATFORM_VERSION, BUFFER_SIZE, buffer, nullptr);
    return std::string(buffer);
}

cl_platform_id Platform::getHandle() const{
    return _handle;
}

void Platform::setHandle(cl_platform_id id){
    _handle = id;
}

Platform::operator cl_platform_id() const{
    return _handle;
}

std::ostream& operator<<(std::ostream& os, const Platform& platform){
    os << "Platform: " << platform.getName() << std::endl;
    os << "          Profile: " << platform.getProfile() << std::endl;
    os << "          Version: " << platform.getVersion() << std::endl;
    os << "          Vendor: " << platform.getVendor() << std::endl;
    os << "          Extensions: " << platform.getExtensions();
    return os;
}

std::vector<Platform> getAllPlatforms(){
    //10 ids is enough to get all the platforms
    static const int MAX_IDS = 10;
    cl_platform_id ids[MAX_IDS];
    unsigned int num_platforms;
    clGetPlatformIDs(MAX_IDS, ids, &num_platforms);

    std::vector<Platform> res(num_platforms);
    for(unsigned int i = 0; i < num_platforms; i++){
        res[i].setHandle(ids[i]);
    }

    return res;
}

}
