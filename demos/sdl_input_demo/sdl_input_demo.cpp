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

  sdl::input input;
  input.enable_unicode(true);
  input.quit_slot().connect( quit );
  input.keyboard_slot().connect( keypress );
  input.mouse_motion_slot().connect( mouse_move );
  input.mouse_button_slot().connect( mouse_clicked );

  ghp::vector<3, float> vertices[] = {
    vector3<float>(1, 0, 0),
    vector3<float>(0, 1, 0),
    vector3<float>(-1, 0, 0) };
  ghp::color<ghp::RGBA<float> > colors[] = {
    ghp::standard_colors<RGBA<float> >::RED,
    ghp::standard_colors<RGBA<float> >::BLUE,
    ghp::standard_colorS<RGBA<float> >::GREEN };
  int indices[] = { 0, 1, 2 };
  gl::vbo<GL_ARRAY_BUFFER> vertex_vbo;
  gl::vbo<GL_ARRAY_BUFFER> color_vbo;
  gl::vbo<GL_ELEMENT_ARRAY_BUFFER> element_vbo;
  gl::texture<2, ghp::RGBA<uint8_t> > gl_texture;

  {
    ghp::texture<ghp::RGBA<uint8_t> > soft_texture;
    ghp::texture<ghp::RGBA<uint8_t> > soft_texture_sized(512, 512);
    sdl::load_texture("texture.png", soft_texture);
    ghp::scale_texture(soft_texture, soft_texture_sized);
    gl_texture.write(soft_texture_sized);
  }

  vertex_vbo.write(vertices, sizeof(vertices));
  color_vbo.write(colors, sizeof(colors));
  element_vbo.write(indices, sizeof(indices));

  gl::display_list dlist;
  dlist.begin(); // default: just compile
  gl::bind_vertex(vertex_vbo);
  gl::bind_color(color_vbo);
  gl::bind_element(element_vbo);
  gl::draw_arrays(GL_TRIANGLES, 3);
  dlist.end();

  while(running) {
    input.handle_events();

    dlist.execute();

    display.update();
  }
  std::cout << "quit!" << std::endl;
}

