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
  while(true) {
  std::list<cl::platform_ref> platforms;
  cl::platform_ref::get_platforms(platforms);

  cl::platform_ref p = platforms.front();
  std::cout << p.name() << " " << p.version() << std::endl;

  std::vector<cl::device_ref> devices;
  p.get_devices(devices);
  for(std::size_t i=0; i<devices.size(); ++i) {
    cl::device_ref d = devices[i];
    std::cout << d.name() << std::endl;
    std::cout << d.extensions() << std::endl;
    std::cout << d.global_mem_cacheline_size() << std::endl;
  }

  cl::device_ref d = devices.front();
  cl::context_ref context(p, d);
  cl::command_queue_ref commands(context, d);
  cl::program_ref program(context, source);
  program.build();
  cl::kernel_ref kernel = program.get_kernel("square");

  cl::buffer_ref input_buffer(context, sizeof(float)*128);
  cl::buffer_ref output_buffer(context, sizeof(float)*128);

  std::vector<float> vecs;
  vecs.reserve(128);
  for(int i=0; i<128; ++i) vecs[i] = i;

  commands.write_buffer(input_buffer, sizeof(float)*128, &vecs[0]).wait();

  kernel.set_arg(0, input_buffer);
  kernel.set_arg(1, output_buffer);
  kernel.set_arg(2, 128);
  std::cout << "work group size: " << kernel.work_group_size(d) << std::endl;

  std::size_t s = 12800;
  std::size_t s2 = 1;

  for(int i=0; i<128; ++i) {
    std::cout << vecs[i] << " ";
  }
  std::cout << std::endl;

  commands.run_kernel(kernel, 1, &s, &s2).wait();
  commands.read_buffer(output_buffer, sizeof(float)*128, &vecs[0]).wait();

  for(int i=0; i<128; ++i) {
    std::cout << vecs[i] << " ";
  }
  std::cout << std::endl;
  }
}

