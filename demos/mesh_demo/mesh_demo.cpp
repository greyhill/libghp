#include <ghp/gfx/sdl.hpp>
#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  ghp::mesh<ghp::ln_vertex<3, float> > mesh;
  ghp::load_obj_mesh("thing.obj", mesh);

  sdl::gl_display display(800, 600, false);
  display.set_caption("mesh_demo");
}

