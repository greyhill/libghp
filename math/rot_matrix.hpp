#ifndef _GHP_MATH_ROT_MATRIX_HPP_
#define _GHP_MATH_ROT_MATRIX_HPP_

#include "vector.hpp"

namespace ghp {

/**
  \brief a simple rotation matrix in R^N.  Users seeking 
  a more general matrix object should check out boost::ublas
  or something similar.  These are matrices with orthonormal
  columns.
  \tparam N - dimension of rot_matrix
  \tparam T - underlying storage type
 */
template<int N, typename T>
class rot_matrix {
public:
  /** create identity rot_matrix */
  rot_matrix() {
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        (*this)(r,c) = (r == c ? 1 : 0);
      }
    }
  }
  /** copy constructor */
  rot_matrix(const rot_matrix &m) {
    for(int i=0; i<N*N; ++i) (*this)(i) = m(i);
  }
  ~rot_matrix() {
  }

  /** element access */
  inline T& operator()(int r, int c) {
    return data_[r*N + c];
  }
  /** element access */
  inline const T& operator()(int r, int c) const {
    return data_[r*N + c];
  }

  /** element access */
  inline T& operator()(int i) {
    return data_[i];
  }
  /** element access */
  inline const T& operator()(int i) const {
    return data_[i];
  }
  /** element access */
  inline T& operator[](int i) {
    return (*this)(i);
  }
  /** element access */
  inline const T& operator[](int i) const {
    return (*this)(i);
  }
  
  inline rot_matrix operator*(const rot_matrix &m) const {
    rot_matrix out;
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        T cell = 0;
        for(int k=0; k<N; ++k) {
          cell += (*this)(r, k) * m(k, c);
        }
        out(r, c) = cell;
      }
    }
    return out;
  }
  inline vector<N, T> operator*(const vector<N, T> &v) const {
    vector<N, T> out;
    for(int r=0; r<N; ++r) {
      T cell = 0;
      for(int k=0; k<N; ++k) {
        cell += v(k) * (*this)(r, k);
      }
      out(r) = cell;
    }
    return out;
  }
  inline vector<N-1, T> operator*(const vector<N-1, T> &v) const {
    vector<N-1, T> out;
    for(int r=0; r<N-1; ++r) {
      T cell = 0;
      for(int k=0; k<N-1; ++k) {
        cell += v(k) * (*this)(r, k);
      }
      out(r) = cell;
    }
    return out;
  }
  inline rot_matrix operator/(const rot_matrix &m) const {
    rot_matrix tmp;
    m.invert(tmp);
    return (*this) * tmp;
  }
  
  inline rot_matrix& operator*=(const rot_matrix &m) {
    rot_matrix tmp( (*this) * m );
    *this = tmp;
    return *this;
  }
  inline rot_matrix& operator/=(const rot_matrix &m) {
    rot_matrix tmp;
    m.invert(tmp);
    return (*this) *= tmp;
  }

  inline rot_matrix invert() const {
    rot_matrix out;
    // transpose
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        out(r,c) = (*this)(c,r);
      }
    }
    return out;
  }
  template<typename T2>
  inline void invert(rot_matrix<N, T2> &out) const {
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        out(r,c) = (*this)(c,r);
      }
    }
  }
  
  template<typename T2, typename T3>
  inline void post_multiply(const vector<N, T2> &v, vector<N, T3> &out) const {
    for(int r=0; r<N; ++r) {
      T cell = 0;
      for(int k=0; k<N; ++k) {
        cell += v(k) * (*this)(r,k);
      }
      out(r) = cell;
    }
  }
  template<typename T2, typename T3>
  inline void post_multiply(const vector<N-1, T2> &v, 
      vector<N-1, T3> &out) const {
    for(int r=0; r<N-1; ++r) {
      T cell = 0;
      for(int k=0; k<N-1; ++k) {
        cell += v(k) * (*this)(r, k);
      }
      out(r) = cell;
    }
  }
  template<typename T2, typename T3>
  inline void post_multiply(const rot_matrix<N, T2> &m, 
      rot_matrix<N, T3> &out) const {
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        T cell = 0;
        for(int k=0; k<N; ++k) {
          cell += (*this)(r, k) * m(k, c);
        }
        out(r,c) = cell;
      }
    }
  }

private:
  T data_[N];
};

}

#endif

