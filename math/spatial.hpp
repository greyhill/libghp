#ifndef _GHP_MATH_SPATIAL_HPP_
#define _GHP_MATH_SPATIAL_HPP_

#include "rot_axis_angle.hpp"
#include "rot_complex.hpp"
#include "rot_euler.hpp"
#include "rot_matrix.hpp"
#include "rot_quat.hpp"
#include "vector.hpp"

#include <complex>
#include <limits>

#include <cmath>

namespace ghp {

// dimensional traits
template<int N, typename T> struct spatial_traits { };
template<typename T> struct spatial_traits<2, T> {
};

// conversion glue
// 2D 
// complex <-> matrix
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
// euler <-> complex 
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
// euler <-> matrix
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

// 3D 
// euler -> matrix
template<typename T1, typename T2>
struct delegated_assignment<rot_matrix<3, T2>, rot_euler<3, T1> > {
  inline void operator()(rot_matrix<3, T2> &m, const rot_euler<3, T1> &r) {
    const T1 cosp = std::cos(r.pitch());
    const T1 sinp = std::sin(r.pitch());
    const T1 cosy = std::cos(r.yaw());
    const T1 siny = std::sin(r.yaw());
    const T1 cosr = std::cos(r.roll());
    const T1 sinr = std::sin(r.roll());

    m(0,0) = cosy*cosr;
    m(1,0) = -sinp*siny*cosr - sinr*cosp;
    m(2,0) = -cosp*siny*cosr + sinp*sinr;

    m(0,1) = cosy*sinr;
    m(1,1) = -sinp*siny*sinr + cosp*cosr;
    m(2,1) = -cosr*sinp - cosp*sinr*siny;

    m(0,2) = siny;
    m(1,2) = sinp*cosy;
    m(2,2) = cosy*cosp;
  }
};

// axis_angle -> matrix
template<typename T1, typename T2>
struct delegated_assignment<rot_matrix<3, T1>, rot_axis_angle<T2> > {
  inline void operator()(rot_matrix<3, T1> &m, const rot_axis_angle<T2> &r) {
    const T2 c = std::cos(r.angle());
    const T2 s = std::sin(r.angle());
    const T2 t = 1 - c;
    const T2 x = r.axis()[0];
    const T2 y = r.axis()[1];
    const T2 z = r.axis()[2];

    m(0,0) = t*x*x + c;
    m(1,0) = t*x*y + z*s;
    m(2,0) = t*x*z - y*s;

    m(0,1) = t*x*y - z*s;
    m(1,1) = t*y*y + c;
    m(2,1) = t*y*z + x*s;

    m(0,2) = t*x*z + y*s;
    m(1,2) = t*y*s - x*s;
    m(2,2) = t*z*z + c;
  }
};

}

#endif

