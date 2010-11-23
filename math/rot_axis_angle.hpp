#ifndef _GHP_MATH_ROT_AXIS_ANGLE_HPP_
#define _GHP_MATH_ROT_AXIS_ANGLE_HPP_

#include "vector.hpp"
#include "../util.hpp"

namespace ghp {

template<typename T>
class rot_axis_angle {
public:
  inline rot_axis_angle(const vector<3, T> &v, const T &radians) 
      : axis_(v),
      angle_(radians) {
  }
  template<typename T2> 
  inline rot_axis_angle(const rot_axis_angle<T2> &r) 
      : axis_(r.axis_),
      angle_(r.angle_) {
  }
  template<typename F>
  inline rot_axis_angle(const F &f) {
    delegated_assignment<rot_axis_angle<T>, F> copy;
    copy(*this, f);
  }
  inline ~rot_axis_angle() { 
  }

  template<typename T2>
  inline rot_axis_angle& operator=(const rot_axis_angle<T2> &r) {
    axis_ = r.axis_;
    angle_ = r.angle_;
    return *this;
  }
  template<typename F>
  inline rot_axis_angle& operator=(const F &f) {
    delegated_assignment<rot_axis_angle<T>, F> copy;
    copy(*this, f);
    return *this;
  }

  inline const vector<3, T>& axis() const { return axis_; }
  inline vector<3, T>& axis() { return axis_; }
  inline const T& angle() const { return angle_; }
  inline T& angle() { return angle_; }

private:
  vector<3, T> axis_;
  T angle_;
};

}

#endif

