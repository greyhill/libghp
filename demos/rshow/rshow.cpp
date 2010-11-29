#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>
#include <ghp/gfx/gl.hpp>

#include <ghp/util/sdl_input.hpp>

volatile bool running = true;

struct tile {
};

bool quit_handler(const sdl::quit_event &evt) {
  (void)evt;
  std::cout << "user quit." << std::endl;
  running = false;
  return 0;
}

void update_physics(float seconds) {
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  sdl::gl_display display(800, 600, false);
  display.enable_vsync(true);
  display.set_caption("ghp rshow demo");
  display.set_target_fps(30);
  gl::init();

  sdl::input input;
  input.quit_slot().connect(quit_handler);

  display.update();

  gl::texture<2, ghp::RGBA<float> > tex;
  gl::bind_texture<>(tex);

  while(running) {
    /* draw scene */


    /* other minutae */
    float secs = display.update();
    input.handle_events();
    update_physics(secs);
  }

  return 0;
}

