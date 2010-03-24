#ifndef _GHP_MATH_INTERPOLATE_HPP_
#define _GHP_MATH_INTERPOLATE_HPP_

namespace ghp {

template<typename T>
inline T linear_interpolate(const T &begin, const T &end, float s) {
  return (end - begin)*s + begin;
}

template<typename T>
inline T linear_interpolate(const T &begin, const T &end, float s_begin,
    float s_end, float s) {
  return linear_interpolate(begin, end, (s-s_begin)/(s_end-s_begin));
}

}

#endif

