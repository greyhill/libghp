#include <ghp/gfx.hpp>
#include <ghp/gfx/gl.hpp>
#include <ghp/gfx/sdl.hpp>

#include <ghp/util.hpp>
#include <ghp/util/sdl_input.hpp>

#include <iostream>

volatile bool running = true;

bool keypress(const sdl::keyboard_event &evt) {
  char c = (char)evt.unicode();
  if(evt.down()) {
    std::cout << "key down: " << c << std::endl;
  } else {
    std::cout << "key up: " << c << std::endl;
  }
  return true;
}

bool quit(const sdl::quit_event &evt) {
  running = false;
  return true;
}

bool mouse_move(const sdl::mouse_motion_event &evt) {
  std::cout << "mouse moved by " << evt.rel_coords<int16_t>() 
      << std::endl;
  return true;
}

bool mouse_clicked(const sdl::mouse_button_event &evt) {
  std::cout << "mouse clicked at " << evt.coords<uint16_t>()
      << std::endl;
  return true;
}

int main(int argc, char *argv[]) {
  sdl::gl_display display(800, 600, false);
  display.set_caption("SDL Input Demo");
  display.target_fps(30);
  display.restrict_fps(true);
  gl::init();

  sdl::input input;
  input.enable_unicode(true);
  input.quit_slot().connect( quit );
  input.keyboard_slot().connect( keypress );
  input.mouse_motion_slot().connect( mouse_move );
  input.mouse_button_slot().connect( mouse_clicked );

  ghp::vector<3, float> vertices[] = {
    ghp::vector3<float>(1, 0, 0),
    ghp::vector3<float>(0, 1, 0),
    ghp::vector3<float>(-1, 0, 0) };
  ghp::color<ghp::RGBA<float> > colors[] = {
    ghp::standard_colors<ghp::RGBA<float> >::RED,
    ghp::standard_colors<ghp::RGBA<float> >::BLUE,
    ghp::standard_colors<ghp::RGBA<float> >::GREEN };
  int indices[] = { 0, 1, 2 };

  gl::vbo<GL_ARRAY_BUFFER> vertices_vbo;
  gl::vbo<GL_ARRAY_BUFFER> colors_vbo;
  gl::vbo<GL_ELEMENT_ARRAY_BUFFER> indices_vbo;

  vertices_vbo.write(vertices, sizeof(vertices));
  colors_vbo.write(colors, sizeof(colors));
  indices_vbo.write(indices, sizeof(indices));

  while(running) {
    input.handle_events();

    gl::vertex_pointer(vertices_vbo);

    display.update();
  }
  std::cout << "quit!" << std::endl;
}

