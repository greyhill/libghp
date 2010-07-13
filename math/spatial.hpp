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
// 2D 
template<typename T1, typename T2>
struct delegated_assignment<rot_matrix<2, T1>, rot_complex<T2> > {
  inline void operator()(rot_matrix<2, T1> &m, const rot_complex<T2> &c) {
    m(0, 0) = c.real();
    m(1, 0) = c.imag();
    m(0, 1) = -c.imag();
    m(1, 1) = c.real();
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_complex<T1>, rot_matrix<2, T2> > {
  inline void operator()(rot_complex<T1> &c, const rot_matrix<2, T2> &m) {
    c.real() = m(0, 0);
    c.imag() = m(1, 0);
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_euler<2, T1>, rot_complex<T2> > {
  inline void operator()(rot_euler<2, T1> &e, const rot_complex<T2> &c) {
    e.rotation() = std::atan2(c.imag(), c.real());
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_complex<T1>, rot_euler<2, T2> > {
  inline void operator()(rot_complex<T1> &c, const rot_euler<2, T2> &e) {
    c.real() = std::cos(e.rotation());
    c.imag() = std::sin(e.rotation());
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_euler<2, T1>, rot_matrix<2, T2> > {
  inline void operator()(rot_euler<2, T1> &e, const rot_matrix<2, T2> &m) {
    e.rotation() = std::atan2(m(1,0), m(0,0));
  }
};
template<typename T1, typename T2>
struct delegated_assignment<rot_matrix<2, T1>, rot_euler<2, T2> > {
  inline void operator()(rot_matrix<2, T1> &m, const rot_euler<2, T2> &e) {
    const T1 c = std::cos(e.rotation());
    const T1 s = std::sin(e.rotation());
    m(0,0) = c;
    m(1,0) = s;
    m(0,1) = -s;
    m(1,1) = c;
  }
};

}

#endif

