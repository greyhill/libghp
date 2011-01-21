#include "div_kernel.opencl.hpp"

#include <boost/progress.hpp>

#include <ghp/util/cl.hpp>

#include <list>
#include <vector>

int main(int argc, char *argv[]) {
  std::list<cl::platform_ref> platforms;
  std::list<cl::device_ref> devices;

  cl::platform_ref::get_platforms(platforms);
  cl::platform_ref platform = platforms.front();

  platform.get_devices(devices);
  cl::device_ref device = devices.front();

  cl::context_ref context(platform, device);
  cl::command_queue_ref queue(context, device, true, true);

  cl::program_ref program(context, div_kernel_opencl_source);
  program.build();

  cl::kernel_ref with_div = program.get_kernel("with_div");
  cl::kernel_ref with_sync = program.get_kernel("with_sync");
  cl::kernel_ref no_div = program.get_kernel("no_div");

  std::vector<float> buffer(1024*768);
  for(int i=0; i<buffer.size(); ++i) {
    buffer[i] = random();
    buffer[i] -= RAND_MAX/2;
  }

  cl::buffer_ref in_buffer(context,
      sizeof(float)*1024*768);
  cl::buffer_ref out_buffer(context,
      sizeof(float)*1024*768);
  std::size_t global_size[] = { 1024, 768 };
  std::size_t local_size[] = { 16, 16 };

  cl::kernel_ref use_program = with_div;
  for(int i=0; i<3; ++i) {
    switch(i) {
      case 0: use_program = with_div; break;
      case 1: use_program = with_sync; break;
      case 2: use_program = no_div; break;
    }
    use_program.set_arg(0, in_buffer);
    use_program.set_arg(1, out_buffer);
    for(int i=0; i<32; ++i) {
      boost::progress_timer timer;
      queue.run_kernel(use_program,
          2,
          global_size,
          local_size).wait();
    }
  }

  return EXIT_SUCCESS;
}

