#ifndef _GHP_MATH_INTERPOLATE_HPP_
#define _GHP_MATH_INTERPOLATE_HPP_

namespace ghp {

template<typename T>
T linear_interpolate(const T &begin, const T &end, float s) {
  return (begin - end)*s + begin;
}

}

#endif

