#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  ghp::mesh<ghp::ln_vertex<3, float> > mesh;
  ghp::load_obj_mesh("thing.obj", mesh);
  std::cout << mesh.num_vertices() << " " << mesh.num_faces()
    << std::endl;
}

