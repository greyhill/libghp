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
  vec.resize(8); vec2.resize(8);
  for(int i=0; i<vec.size(); ++i) vec[i] = std::complex<double>(i, 0);
  for(int i=0; i<vec2.size(); ++i) vec2[i] = std::complex<double>(0, 0);

  print_vec(vec, vec.size());
  print_vec(vec2, vec2.size());
  std::cout << std::endl;

  fftw::fft(vec, vec);
  fftw::ifft(vec, vec);
  print_vec(vec, vec.size());
  print_vec(vec2, vec2.size());
}

