#ifndef _GHP_MATH_VERTEX_AUX_HPP_
#define _GHP_MATH_VERTEX_AUX_HPP_

namespace ghp {

template<typename V> struct vertex_write_loc {
  template<typename S> inline void operator()(V &v, const S &s) { }
};
template<typename V> struct vertex_read_loc {
  template<typename S> inline void operator()(const V &v, S &s) { }
};

template<typename V> struct vertex_write_norm {
  template<typename S> inline void operator()(V &v, const S &s) { }
};
template<typename V> struct vertex_read_norm {
  template<typename S> inline void operator()(const V &v, S &s) { }
};

template<typename V> struct vertex_write_uv {
  template<typename S> inline void operator()(V &v, const S &s) { }
};
template<typename V> struct vertex_read_uv {
  template<typename S> inline void operator()(const V &v, S &s) { }
};

}

#endif

