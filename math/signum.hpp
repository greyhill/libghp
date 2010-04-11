#ifndef _GHP_MATH_SIGNUM_HPP_
#define _GHP_MATH_SIGNUM_HPP_

namespace ghp {

/**
  \brief mathematical signum function
  \tparam T - "1", "0" and "-1" can be meaningfully cast to it
  \param t - value to test 
  \return 0 if t == 0, -1 if t is negative, 1 if t is positive
 */
template<typename T>
inline int signum(const T &t) {
  return t > T(0) ? T(1) : (t == T(0) ? T(0) : T(-1));
}

}

#endif

