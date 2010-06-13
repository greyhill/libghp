#include <ghp/math/fftw.hpp>

#include <complex>
#include <iostream>
#include <vector>

template<typename T>
inline void print_vec(T &t, std::size_t size) {
  for(std::size_t i=0; i<size; ++i) {
    std::cout << t[i] << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  std::vector<std::complex<double> > vec;
  std::vector<std::complex<double> > vec2;
  std::vector<std::complex<double> > vec3;
  vec.resize(3); vec2.resize(3);
  vec3.resize(5);
  vec[0] = 1; vec[1] = 1; vec[2] = 1;
  vec2[0] = 1; vec2[1] = 2; vec2[2] = 1;

  print_vec(vec, vec.size());
  print_vec(vec2, vec2.size());
  print_vec(vec3, vec3.size());
  std::cout << std::endl;

  fftw::conv(vec, vec2, vec3);
  print_vec(vec, vec.size());
  print_vec(vec2, vec2.size());
  print_vec(vec3, vec3.size());
}

