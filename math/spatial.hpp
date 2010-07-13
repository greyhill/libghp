#ifndef _GHP_MATH_SPATIAL_HPP_
#define _GHP_MATH_SPATIAL_HPP_

#include "vector.hpp"
#include "rot_axis_angle.hpp"
#include "rot_complex.hpp"
#include "rot_euler.hpp"
#include "rot_matrix.hpp"
#include "rot_quat.hpp"

#include <complex>
#include <limits>

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
template<typename T1, typename T2>
struct delegated_assignment<rot_matrix<2, T1>, rot_complex<T2> > {
  inline void operator()(rot_matrix<2, T1> &m, rot_complex<T2> &c) {
    m(0, 0) = c.real();
    m(1, 0) = c.imag();
    m(0, 1) = -c.imag();
    m(1, 1) = c.real();
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_complex<T1>, rot_matrix<2, T2> > {
  inline void operator()(rot_complex<T1> &c, rot_matrix<2, T2> &m) {
    c.real() = m(0, 0);
    c.imag() = m(1, 0);
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_euler<2, T1>, rot_complex<T2> > {
  inline void operator()(rot_euler<2, T1> &e, rot_complex<T2> &c) {
    T2 angle;
    if(std::abs(c.real()) < std::numeric_limits<T2>::epsilon()) {
      angle = M_PI / 2.0;
    } else {
      angle = std::atan(c.imag() / c.real());
    }
    if(c.real() < 0) {
      angle += M_PI;
    }
    e.angle() = angle;
  }
};

}

#endif

