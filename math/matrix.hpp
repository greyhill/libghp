#ifndef _GHP_MATRIX_HPP_
#define _GHP_MATRIX_HPP_

#include "vector.hpp"

namespace ghp {

/**
  \brief simple dense square NxN matrix class
  \tparam N - dimension of matrix
  \tparam T - underlying storage type
 */
template<int N, typename T>
class matrix {
public:
  /** create identity matrix */
  matrix() {
    for(int r=0; r<N; ++r) {
      for(int c=0; c<N; ++c) {
        (*this)(r,c) = (r == c ? 1 : 0);
      }
    }
  }
  /** copy constructor */
  matrix(const matrix &m) {
    for(int i=0; i<N*N; ++i) (*this)(i) = m(i);
  }
  ~matrix() {
  }

  /** element access */
  inline T& operator()(int r, int c) {
    return data_[r*N + c];
  }
  /** element access */
  inline const T& operator()(int r, int c) {
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
  
  inline matrix operator+(const matrix &m) const {
    matrix out;
    for(int i=0; i<N*N; ++i) {
      out(i) = (*this)(i) + m(i);
    }
    return out;
  }
  inline matrix operator-(const matrix &m) const {
    matrix out;
    for(int i=0; i<N*N; ++i) {
      out(i) = (*this)(i) - m(i);
    }
    return out;
  }
  inline matrix operator*(const matrix &m) const {
    matrix out;
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
    for(int r=0; r<N; ++r0) {
      T cell = 0;
      for(int k=0; k<N; ++k) {
        cell += v(k) * (*this)(r, k);
      }
      out(r) = cell;
    }
    return out;
  }

  inline matrix& operator+=(const matrix &m) {
    for(int i=0; i<N*N; ++i) {
      (*this)(i) += m(i);
    }
    return *this;
  }
  inline matrix& operator-=(const matrix &m) {
    for(int i=0; i<N*N; ++i) {
      (*this)(i) -= m(i);
    }
    return *this;
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
  inline void post_multiply(const matrix<N, T2> &m, matrix<N, T3> &out) const {
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

