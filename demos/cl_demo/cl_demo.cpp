#include <ghp/util/cl.hpp>
#include <ghp/util/sdl_input.hpp>

#include <ghp/gfx.hpp>
#include <ghp/gfx/gl.hpp>
#include <ghp/gfx/sdl.hpp>

#include <iostream>
#include <list>
#include <vector>

#include "kernel.opencl.hpp"

volatile bool running = true;

bool handle_quit(const sdl::quit_event &q) {
  running = false;
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<cl::platform_ref> platforms;
  std::vector<cl::device_ref> devices;
  cl::platform_ref::get_platforms(platforms);
  platforms[0].get_devices(devices);

  std::cout << "want to create OpenCL context using platform " 
    << platforms[0].name() << "/" << devices[0].name() << std::endl;

  cl::context_ref context(platforms[0], devices[0]);
  cl::command_queue_ref command(context, devices[0]);
  cl::program_ref program(context, kernel_opencl_source);
  program.build();
  cl::kernel_ref kernel = program.get_kernel("kernel_main");

  typedef ghp::Single<0, ghp::RGB<float> > PIXELT;

  ghp::texture<PIXELT> tex;
  sdl::load_image("img.jpg", tex);
  cl::image2d_ref cl_img(context, PIXELT(), 512, 512);
  cl::image2d_ref cl_out(context, PIXELT(), 512, 512);
  command.write_image2d(cl_img,
      ghp::vector2<std::size_t>(512, 512),
      &tex[0]).wait();

  kernel.set_arg(0, 512);
  kernel.set_arg(1, 512);
  kernel.set_arg(2, cl_img);
  kernel.set_arg(3, cl_out);
  std::size_t global_sizes[] = { 512, 512 };
  std::size_t local_sizes[] = { 16, 16 };
  command.run_kernel(kernel, 2, global_sizes, local_sizes).wait();
  
  command.read_image2d(cl_out, &tex[0], ghp::vector2<std::size_t>(512, 512));
  
  float max = 0;
  for(int i=0; i<512*512; ++i) {
    max = std::max(max, tex[i][0]);
  }
  std::cout << "max value: " << max << std::endl;
  sdl::save_bmp("out.bmp", tex);
}

