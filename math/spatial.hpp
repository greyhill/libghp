#ifndef _GHP_MATH_SPATIAL_HPP_
#define _GHP_MATH_SPATIAL_HPP_

#include "vector.hpp"
#include "rot_complex.hpp"
#include "rot_euler.hpp"
#include "rot_matrix.hpp"
#include "rot_quat.hpp"

#include <complex>

#include <cmath>

namespace ghp {

template<int N, typename T> struct spatial_traits { };
template<typename T> struct spatial_traits<2, T> {
  typedef T scalar_t;
  typedef vector<2, T> vector_t;
  typedef rot_complex<T> stable_rot_t;
  typedef rot_matrix<2, T> matrix_rot_t;
  typedef rot_matrix<3, T> matrix_trans_t;
};

// conversion glue
template<typename T>
struct delegated_assignment<rot_matrix<2, T>, rot_complex<T> > {
  delegated_assignment(rot_matrix<2, T> &m, rot_complex<T> &c)
      : m_(m), c_(c) { }
  void operator()() {
  }
  rot_matrix<2, T> &m_;
  rot_complex<T> &c_;
};

}

#endif

