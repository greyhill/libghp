#include <ghp/gfx.hpp>
#include <ghp/gfx/gl.hpp>
#include <ghp/gfx/sdl.hpp>

#include <ghp/util/sdl_input.hpp>

#include <ghp/math.hpp>

#include <iostream>
#include <string>

volatile bool running = true;

bool handle_quit(const sdl::quit_event &evt) {
  running = false;
  return true;
}

bool handle_key(const sdl::keyboard_event &evt) {
  if(evt.up()) {
    if(evt.keysym() == SDLK_ESCAPE) {
      running = false;
    }
  }
  return true;
}

struct vertex {
  ghp::vector<3, float> loc;
  ghp::vector<2, float> tex;
};

int main(int argc, char *argv[]) {
  if(argc < 2) {
    std::cerr << "usage: " << argv[0] << " IMAGE"
      << std::endl;
    return EXIT_FAILURE;
  }
  std::string img_name(argv[1]);

  sdl::gl_display display(800, 600, false);
  display.set_caption("vbo demo");
  gl::init();

  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  ghp::texture<ghp::RGBA<uint8_t> > raw_pic;
  sdl::load_image(img_name, raw_pic);
  
  gl::texture<2, ghp::RGB<uint8_t> > texture;
  texture.write(raw_pic);
  gl::bind_texture<>(texture);

  vertex vertices[4];
  vertices[0].loc = ghp::vector3<float>(raw_pic.get_width()/2, 
      -raw_pic.get_height()/2, 0);
  vertices[0].tex = ghp::vector2<float>(1, 1);
  vertices[1].loc = ghp::vector3<float>(raw_pic.get_width()/2,
      raw_pic.get_height()/2, 0);
  vertices[1].tex = ghp::vector2<float>(1, 0);
  vertices[2].loc = ghp::vector3<float>(-raw_pic.get_width()/2,
      raw_pic.get_height()/2, 0);
  vertices[2].tex = ghp::vector2<float>(0, 0);
  vertices[3].loc = ghp::vector3<float>(-raw_pic.get_width()/2,
      -raw_pic.get_height()/2, 0);
  vertices[3].tex = ghp::vector2<float>(0, 1);

  gl::vbo<GL_ARRAY_BUFFER> object_vbo;
  object_vbo.write(vertices, sizeof(vertices));

  uint32_t idx[] = { 0, 1, 3, 1, 2, 3 };
  
  gl::vbo<GL_ELEMENT_ARRAY_BUFFER> element_vbo;
  element_vbo.write(idx, sizeof(idx));

  sdl::input input;
  input.quit_slot().connect(handle_quit);
  input.keyboard_slot().connect(handle_key);

  while(running) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-display.get_width()/2, display.get_width()/2, 
        -display.get_height()/2, display.get_height()/2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gl::bind_buffer(object_vbo);
    gl::vertex_pointer<float>(3, sizeof(vertex), 0);
    gl::tex_coord_pointer<float>(2, sizeof(vertex), 3*sizeof(float));
    gl::bind_buffer(element_vbo);
    gl::draw_elements<uint32_t>(GL_TRIANGLES, 2*3, 0);

    display.update();
    input.handle_events();
  }
}

