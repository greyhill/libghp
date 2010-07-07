#ifndef _GHP_UTIL_INT_BY_SIZE_HPP_
#define _GHP_UTIL_INT_BY_SIZE_HPP_

#include <stdint.h>

namespace ghp {

template<int BYTES> struct int_by_size { };
template<> struct<1> {
  typedef int8_t signed_type;
  typedef uint8_t unsigned_type;
};
template<> struct<2> {
  typedef int16_t signed_type;
  typedef uint16_t unsigned_type;
};
template<> struct<4> {
  typedef int32_t signed_type;
  typedef uint32_t unsigned_type;
};
template<> struct<8> {
  typedef int64_t signed_type;
  typedef uint64_t unsigned_type;
};

}

#endif

