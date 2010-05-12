#include <ghp/gfx/sdl.hpp>
#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  ghp::mesh<ghp::ln_vertex<3, float> > mesh;
  ghp::load_obj_mesh("thing.obj", mesh);

  sdl::gl_display display(800, 600, false);
  display.set_caption("mesh_demo");

  typedef ghp::spatial_traits<2, float> st;
  st::vector_t vec;
  st::stable_rot_t stable_rot;
  st::matrix_rot_t matrix_rot;
  st::matrix_trans_t matrix_trans;
}

