#ifndef _GHP_MATH_ROT_AXIS_ANGLE_HPP_
#define _GHP_MATH_ROT_AXIS_ANGLE_HPP_

#include "vector.hpp"
#include "../util.hpp"

namespace ghp {

template<typename T>
class rot_axis_angle {
public:
  inline rot_axis_angle(const vector<3, T> &v, const T &radians);
  template<typename T2> 
  inline rot_axis_angle(const rot_axis_angle<T2> &r);
  template<typename F>
  inline rot_axis_angle(const F &f);
};

}

#endif

