#include <iostream>
#include "compute/compute.hpp"

#define DATA_SIZE (1024*1024)

const std::string KernelSource = "\n"		    \
    "__kernel void square(                    \n" \
    "   __global float* input,                \n" \
    "   __global float* output,               \n" \
    "   const unsigned int count)             \n" \
    "{                                        \n" \
    "   int i = get_global_id(0);             \n" \
    "   if(i < count)                         \n" \
    "       output[i] = input[i] * input[i];  \n" \
    "}                                        \n" \
    "\n";

int main(int argc, char* argv[]){
    //Get the platform and device
    auto platforms = compute::getAllPlatforms();
    auto devices = compute::getAllDevices(platforms[0]);
    auto device = devices[0];

    std::cout << platforms[0] << std::endl;
    std::cout << devices[0] << std::endl;

    //Create the context (in a broad sense)
    auto context = compute::Context(device);
    auto commands = compute::CommandQueue(context);

    //Make the program
    auto program = context.programFromSource(KernelSource);
    program.build();

    if(!program.isBuilt()){
        std::cerr << "Error building program" << std::endl;
        std::cerr << program.getBuildLog() << std::endl;
        exit(1);
    }

    auto kernel = program.getKernel("square");

    //Create host side buffers
    float* h_input = new float[DATA_SIZE];
    float* h_results = new float[DATA_SIZE];

    //Fill the input with random data
    for(int i = 0; i < DATA_SIZE; i++){
        h_input[i] = rand() / (float) RAND_MAX;
    }

    //Create the device buffers
    auto d_input = context.createBuffer(sizeof(float) * DATA_SIZE);
    auto d_output = context.createBuffer(sizeof(float) * DATA_SIZE);

    //Upload the input
    commands.upload(h_input, d_input).apply();

    //Launch the kernel
    kernel.setArgs(d_input, d_output, DATA_SIZE);
    commands.range(kernel).global(DATA_SIZE).local(kernel.getWorkGroupSize()).apply();
    commands.waitFinish();

    //Download the result
    commands.download(d_output, h_results).apply();

    //Check the result
    int n_correct = 0;
    for(int i = 0; i < DATA_SIZE; i++){
        if(h_results[i] == h_input[i] * h_input[i]){
            n_correct ++;
        }
    }

    //Print the result
    std::cout << "Number of errors: " << n_correct << "/" << DATA_SIZE << std::endl;

    //Cleanup
    delete[] h_input;
    delete[] h_results;

    return 0;
}
