#ifndef COMPUTE_ENUMS_HPP
#define COMPUTE_ENUMS_HPP

#include <CL/cl.h>

namespace compute{

//Device related enums

enum class DeviceType: unsigned int{
    Accelerator = CL_DEVICE_TYPE_ACCELERATOR,
    All = CL_DEVICE_TYPE_ALL,
    CPU = CL_DEVICE_TYPE_CPU,
    Default = CL_DEVICE_TYPE_DEFAULT,
    GPU = CL_DEVICE_TYPE_GPU,
};

}

#endif //COMPUTE_TEXTURE_HPP
