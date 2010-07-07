#ifndef _GHP_UTIL_INT_BY_SIZE_HPP_
#define _GHP_UTIL_INT_BY_SIZE_HPP_

#include <stdint.h>

namespace ghp {

template<int BYTES> struct int_by_size { };
template<> struct int_by_size<1> {
  typedef int8_t signed_type;
  typedef uint8_t unsigned_type;
};
template<> struct int_by_size<2> {
  typedef int16_t signed_type;
  typedef uint16_t unsigned_type;
  static const unsigned_type upper = 0xFF00;
  static const unsigned_type lower = 0x00FF;
};
template<> struct int_by_size<4> {
  typedef int32_t signed_type;
  typedef uint32_t unsigned_type;
  static const unsigned_type upper = 0xFFFF0000;
  static const unsigned_type lower = 0x0000FFFF;
};
template<> struct int_by_size<8> {
  typedef int64_t signed_type;
  typedef uint64_t unsigned_type;
  static const unsigned_type upper = 0xFFFFFFFF00000000;
  static const unsigned_type lower = 0x00000000FFFFFFFF;
};

}

#endif

