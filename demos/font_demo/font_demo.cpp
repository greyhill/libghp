#include <ghp/gfx.hpp>
#include <ghp/gfx/sdl.hpp>
#include <ghp/gfx/gl.hpp>

#include <ghp/math.hpp>

#include <ghp/util/sdl_input.hpp>

#include <iostream>
#include <string>

volatile bool running = true;

bool handle_quit(const sdl::quit_event &evt) {
  (void)evt;
  running = false;
  return true;
}

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cerr << "usage: " << argv[0] << " FONTFILE " 
        << " MESSAGE " << std::endl;
    return EXIT_FAILURE;
  }

  sdl::gl_display display(800, 600, false);
  gl::init();
  display.set_caption("ghp font demo");

  sdl::input input;
  input.quit_slot().connect(handle_quit);

  ghp::bitmap_font<ghp::RGBA<uint8_t> > font;
  sdl::load_ttf_font_ascii(argv[1], 48, 
      ghp::standard_colors<ghp::RGBA<uint8_t> >::WHITE,
      font);
  gl::bitmap_font<ghp::RGBA<uint8_t> > gl_font(font);

  return EXIT_SUCCESS;

  while(running) {
    input.handle_events();
    display.update();
  }
}

