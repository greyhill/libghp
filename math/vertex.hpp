#ifndef _GHP_MATH_VERTEX_HPP_
#define _GHP_MATH_VERTEX_HPP_

#include "vector.hpp"
#include "vertex_aux.hpp"

namespace ghp {

/**
  \brief vertex containing location and normal information
  \tparam N - dimension of space vertex exists in
  \tparam T - underlying floating point type
 */
template<int N, typename T>
class ln_vertex {
public:
  typedef vector<N, T> vector_t;
  typedef int uv_t; // massively dummy type

  /** \brief create a new ln_vertex */
  ln_vertex() { }
  /** \brief create a new ln_vertex */
  ln_vertex(const vector_t &loc, const vector_t &norm)
      : loc_(loc), norm_(norm) { }
  ~ln_vertex() { }

  /** \brief element access */
  inline vector_t& location() { return loc_; }
  /** \brief element access */
  inline const vector_t& location() const { return loc_; }
  /** \brief element access */
  inline vector_t& normal() { return norm_; }
  /** \brief element access */
  inline const vector_t& normal() const { return norm_; }

private:
  vector_t loc_;
  vector_t norm_;
};

//
// adapters/template magic for ln_vertex 
template<int N, typename T>
struct vertex_write_loc<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(ln_vertex<N, T> &v, const S &s) {
    v.location() = s;
  }
};
template<int N, typename T>
struct vertex_read_loc<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(const ln_vertex<N, T> &v, S &s) {
    s = v.location();
  }
};

template<int N, typename T>
struct vertex_write_norm<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(ln_vertex<N, T> &v, const S &s) {
    v.normal() = s;
  }
};
template<int N, typename T>
struct vertex_read_norm<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(const ln_vertex<N, T> &v, S &s) {
    s = v.normal();
  }
};

template<typename V>
class vertex : public V {
public:
  typedef V backend_t;

  vertex() { }
  template<typename V2> vertex(const vertex<V2> &v) {
    *this = v;
  }
  ~vertex() { }

  template<typename V2>
  inline operator vertex<V2>() const {
    vertex<V2> vert;
    typename V::vector_t vec;
    typename V::uv_t uv;

    vertex_read_loc<V>(*this, vec);
    vertex_write_loc<V2>(vert, vec);
    vertex_read_norm<V>(*this, vec);
    vertex_write_norm<V2>(vert, vec);
    vertex_read_uv<V>(*this, uv);
    vertex_write_uv<V2>(vert, vec);

    return vert;
  }

  template<typename V2>
  vertex& operator=(const vertex<V2> &v) {
    typename V2::vector_t vec;
    typename V::uv_t uv;

    vertex_read_loc<V2>(v, vec);
    vertex_write_loc<V>(*this, vec);
    vertex_read_norm<V2>(v, vec);
    vertex_write_norm<V>(*this, vec);
    vertex_read_uv<V2>(v, uv);
    vertex_write_uv<V>(*this, uv);
  }
};

}

#endif

