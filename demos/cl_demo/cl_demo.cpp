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

  sdl::gl_display display(800, 600, false);
  sdl::input input;
  input.quit_slot().connect(handle_quit);

  gl::texture<2, ghp::RGBA<float> > gl_texture;
  gl_texture.resize(512, 512);

  cl::image2d_ref cl_texture(context, gl_texture);

  command.acquire_gl_objects(&cl_texture, &cl_texture+1).wait();
  std::size_t global_sizes[] = { 512, 512 };
  std::size_t local_sizes[] = { 16, 16 };
  command.run_kernel(kernel, 2, global_sizes, local_sizes).wait();
  command.release_gl_objects(&cl_texture, &cl_texture+1).wait();

  gl::clear_color(ghp::standard_colors<ghp::RGBA<float> >::BLACK);

  while(running) {
    input.handle_events();
    glClear(GL_COLOR_BUFFER_BIT);

    display.update();
  }
}

