#include <ghp/util/cl.hpp>
#include <ghp/util/sdl_input.hpp>

#include <ghp/gfx.hpp>
#include <ghp/gfx/gl.hpp>
#include <ghp/gfx/sdl.hpp>

#include <iostream>
#include <list>
#include <vector>

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

  sdl::gl_display display(800, 600, false);
  sdl::input input;
  input.quit_slot().connect(handle_quit);

  while(running) {
    input.handle_events();
    display.update();
  }
}

