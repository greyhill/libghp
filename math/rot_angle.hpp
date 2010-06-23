#ifndef _GHP_MATH_ROT_ANGLE_HPP_
#define _GHP_MATH_ROT_ANGLE_HPP_

#include <iostream>

namespace ghp {

template<typename T> class rot_degrees;
template<typename T> class rot_radians;

template<typename T> class rot_degrees {
public:
  inline rot_degrees(const T &t = T()) 
      : value_(t) { }
  template<typename T2>
  inline rot_degrees(const rot_degrees<T2> &r) 
      : value_(r.value_) { }
  template<typename T2>
  inline rot_degrees(const rot_radians<T2> &r) 
      : value_(r.to_degrees()) { }
  inline ~rot_degrees() { }

  inline T to_radians() const {
    return value_ * M_PI / 180.0;
  }
  inline T to_degrees() const {
    return value_;
  }
  inline rot_degrees& from_radians(const T &t) {
    value_ = t * 180.0 / M_PI;
    return *this;
  }
  inline rot_degrees& from_degrees(const T &t) {
    value_ = t;
    return *this;
  }

  rot_degrees& operator=(const T &t) {
    value_ = t;
    return *this;
  }
  template<typename T2>
  rot_degrees& operator=(const rot_radians<T2> &r) {
    value_ = r.to_degrees();
    return *this;
  }
  template<typename T2>
  rot_degrees& operator=(const rot_degrees<T2> &r) {
    value_ = r.value_;
    return *this;
  }

  T operator T() const {
    return value_;
  }
private:
  T value_;
};

template<typename T>
inline std::ostream& operator<<(std::ostream &o, const rot_degrees<T> &r) {
  o << r.to_degrees() << " degrees";
}

template<typename T> class rot_radians {
public:
  inline rot_radians(const T &t = T())
      : value_(t) { }
  template<typename T2>
  inline rot_radians(const rot_radians<T2> &r)
      : value_(r.value_) { }
  template<typename T2>
  inline rot_radians(const rot_degrees<T2> &r)
      : value_(r.to_radians()) { }
  inline ~rot_radians() { }

  inline T to_radians() const { return value_; }
  inline T to_degrees() const { return 180.0 / M_PI * value_; }
  inline rot_radians& from_radians(const T &t) { 
    value_ = t;
    return *this;
  }
  inline rot_radians& from_degrees(const T &t) {
    value_ = M_PI 
};

template<typename T>
std::ostream& operator<<(std::ostream &o, const rot_radians<T> &r) {
  o << r.to_radians() << " radians";
}

}

#endif

