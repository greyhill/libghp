#ifndef _GHP_MATH_VERTEX_HPP_
#define _GHP_MATH_VERTEX_HPP_

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

template<int N, typename T>
struct vector_write_loc<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(ln_vertex<N, T> &v, const S &s) {
    v.location() = s;
  }
};

template<int N, typename T>
struct vector_write_norm<ln_vertex<N, T> > {
  template<typename S>
  inline void operator()(ln_vertex<N, T> &v, const S &s) {
    v.normal() = s;
  }
};

}

#endif

