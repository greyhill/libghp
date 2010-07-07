#ifndef _GHP_UTIL_ENDIAN_HPP_
#define _GHP_UTIL_ENDIAN_HPP_

#include "int_by_size.hpp"

#include <stdint.h>

namespace ghp {

const uint16_t ghp_endian_test_val_ = 0xC0DE;
inline bool little_endian() {
  static bool result;
  static bool has_run = false;
  if(!has_run) {
    const uint8_t *i = 
      reinterpret_cast<const uint8_t*>(&ghp_endian_test_val_);
    result = (*i == 0xDE);
    has_run = true;
  }
  return result;
}
inline bool big_endian() {
  return !little_endian();
}

template<int N> struct hton_recursive {
  typedef typename int_by_size<N>::unsigned_type full_type;
  typedef typename int_by_size<N/2>::unsigned_type half_type;

  template<typename T> inline T operator()(const T &t) const {
    const full_type full_buffer = *reinterpret_cast<const full_type*>(&t);
    const half_type upper_buffer = upper_(full_buffer);
    const half_type lower_buffer = lower_(full_buffer);
    hton_recursive<N/2> next_op;
    const half_type upper_flipped = next_op(upper_buffer);
    const half_type lower_flipped = next_op(lower_buffer);
    const full_type full_flipped = 
      (static_cast<full_type>(lower_flipped) << (8*(N/2))) | 
      (static_cast<full_type>(upper_flipped) );
    return *reinterpret_cast<const T*>(&full_flipped);
  }
private:
  inline half_type upper_(const full_type &t) const {
    return static_cast<half_type>(
      (t & int_by_size<N>::upper) >> (8*(N/2)));
  }
  inline half_type lower_(const full_type &t) const {
    return static_cast<half_type>(t & int_by_size<N>::lower);
  }
};
template<> struct hton_recursive<1> {
  template<typename T> inline T operator()(const T &t) const { 
    return t; 
  }
};

template<typename T>
inline T hton(const T &t) {
  if(big_endian()) return t;
  hton_recursive<sizeof(T)> rec;
  return rec(t);
}
template<typename T> inline T ntoh(const T &t) { return hton(t); }

}

#endif

