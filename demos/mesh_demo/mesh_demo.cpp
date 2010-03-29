#include <ghp/math.hpp>

int main(int argc, char *argv[]) {
  ghp::mesh<float> mesh;
  ghp::load_obj_mesh("thing.obj", mesh);
  mesh.check();
}

