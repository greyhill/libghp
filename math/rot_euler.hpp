#ifndef _GHP_MATH_ROT_EULER_HPP_
#define _GHP_MATH_ROT_EULER_HPP_

#include "../util.hpp"

namespace ghp {

template<int N, typename T> class rot_euler { };

template<typename T>
class rot_euler<2, T> {
public:
  inline rot_euler() {
  }
  inline rot_euler(const T &t) {
    angles_[0] = t;
  }
  inline rot_euler(float f) {
    angles_[0] = f;
  }
  inline rot_euler(double d) {
    angles_[0] = d;
  }
  template<typename F> 
  inline rot_euler(const F &f) {
    delegated_assignment<rot_euler<2, T>, F> copy;
    copy(*this, f);
  }
  inline ~rot_euler() {
  }

  template<typename T2>
  inline rot_euler& operator=(const rot_euler<2, T2> &r) {
    angles_[0] = r.angles_[0];
    return *this;
  }
  template<typename F> 
  inline rot_euler& operator=(const F &f) {
    delegated_assignment<rot_euler<2, T>, F> copy;
    copy(*this, f);
    return *this;
  }

  inline const T& operator()(int i) const { return angles_[i]; }
  inline T& operator()(int i) { return angles_[i]; }
  inline const T& operator[](int i) const { return angles_[i]; }
  inline T& operator[](int i) { return angles_[i]; }

  inline const T& rotation() const { return angles_[0]; }
  T& rotation() { return angles_[0]; }

private:
  T angles_[1];
};

template<typename T>
class rot_euler<3, T> {
public:
  inline rot_euler() {
    angles_[0] = angles_[1] = angles_[2] = 0;
  }
  inline rot_euler(const T &pitch, 
      const T &yaw, 
      const T &roll) {
    angles_[0] = pitch;
    angles_[1] = yaw;
    angles_[2] = roll;
  }
  template<typename F> inline rot_euler(const F &f) {
    delegated_assignment<rot_euler<3, T>, F> copy;
    copy(*this, f);
  }
  inline ~rot_euler() {
  }

  template<typename T2>
  inline rot_euler operator=(const rot_euler<3, T2> &r) {
    for(int i=0; i<3; ++i) angles_[i] = r.angles_[i];
    return *this;
  }
  template<typename F>
  inline rot_euler& operator=(const F &f) {
    delegated_assignment<rot_euler<3, T>, F> copy;
    copy(*this, f);
    return *this;
  }

  inline const T& operator()(int i) const { return angles_[i]; }
  inline T& operator()(int i) { return angles_[i]; }
  inline const T& operator[](int i) const { return angles_[i]; }
  inline T& operator[](int i) { return angles_[i]; }

  inline const T& pitch() const { return angles_[0]; }
  inline T& pitch() { return angles_[0]; }
  inline const T& yaw() const { return angles_[1]; }
  inline T& yaw() { return angles_[1]; }
  inline const T& roll() const { return angles_[2]; }
  inline T& roll() { return angles_[2]; }

private:
  T angles_[3];
};

}

#endif

