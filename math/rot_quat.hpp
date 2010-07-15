#ifndef _GHP_MATH_ROT_QUAT_HPP_
#define _GHP_MATH_ROT_QUAT_HPP_

#include "../util.hpp"

namespace ghp {

template<typename T>
class rot_quat {
public:
  inline rot_quat();
  template<typename T2>
  inline rot_quat(const rot_quat<T2> &q);
  template<typename F>
  inline rot_quad(const F &f) {
    delegated_assignment<rot_quat<T>, F> ctor;
    ctor(*this, f);
  }

private:
};

}

#endif

