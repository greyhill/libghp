#ifndef _GHP_MATH_VERTEX_AUX_HPP_
#define _GHP_MATH_VERTEX_AUX_HPP_

namespace ghp {

template<typename V> struct vector_write_loc {
  template<typename S> inline void operator()(V &v, const S &s) { }
};

template<typename V> struct vector_write_norm {
  template<typename S> inline void operator()(V &v, const S &s) { }
};

template<typename V> struct vector_write_uv {
  template<typename S> inline void operator()(V &v, const S &s) { }
};

}

#endif

