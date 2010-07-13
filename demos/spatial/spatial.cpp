#include <ghp/math.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  ghp::rot_euler<2, float> e(M_PI/2.0);
  ghp::rot_matrix<2, float> m = e;
  ghp::rot_complex<float> c = m;
  m = c;
  std::cout << e.rotation() << std::endl;
  e = m;
  c = e;
  e = c;
  std::cout << e.rotation() << std::endl;
}

