#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>

#include <ghp/util/sdl_input.hpp>

volatile bool running = true;

bool quit_handler(const sdl::quit_event &evt) {
  (void)evt;
  std::cout << "user quit." << std::endl;
  running = false;
  return 0;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  sdl::gl_display display(800, 600, false);
  display.enable_vsync(true);
  display.set_caption("ghp rshow demo");
  display.set_target_fps(30);

  sdl::input input;
  input.quit_slot().connect(quit_handler);

  while(running) {
    display.update();
    input.handle_events();
  }

  return 0;
}

