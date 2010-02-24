#include <ghp/gfx.hpp>

int main(int argc, char *argv[]) {
  ghp::display display(800, 600);
  display.set_caption("libghp VBO demo");
  ghp::vertex_buffer<3, float> vbo;
  vbo.resize(1024);
}

