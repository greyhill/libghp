#include <ghp/util/cl.hpp>

#include <iostream>
#include <list>
#include <vector>

const char *source = "\n" \
 "__kernel void square(                                                 \n" \ 
 "   __global float* input,                                              \n" \
 "   __global float* output,                                             \n" \
 "   const unsigned int count)                                           \n" \
 "{                                                                      \n" \
 "   int i = get_global_id(0);                                           \n" \
 "   if(i < count)                                                       \n" \
 "       output[i] = input[i] * input[i];                                \n" \
 "}                                                                      \n" \
 "\n";

int main(int argc, char *argv[]) {
  std::list<cl::platform> platforms;
  cl::platform::get_platforms(platforms);

  cl::platform p = platforms.front();
  std::cout << p.name() << " " << p.version() << std::endl;

  std::vector<cl::device> devices;
  p.get_devices(devices);
  for(std::size_t i=0; i<devices.size(); ++i) {
    cl::device d = devices[i];
    std::cout << d.name() << std::endl;
    std::cout << d.extensions() << std::endl;
    std::cout << d.global_mem_cacheline_size() << std::endl;
  }

  cl::context context(p, devices.front());
  cl::program program(context, source, "");
}

