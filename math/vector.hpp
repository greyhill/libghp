#ifndef GHP_MATH_VECTOR_HPP_
#define GHP_MATH_VECTOR_HPP_

#include <iostream>

#include <cmath>
#include <stdint.h>

namespace ghp { 

/**
  \brief a simple mathematical vector
  \tparam N - dimension of vector
  \tparam T - underlying type
 */
template<int N, typename T>
class vector {
public:
  /** create a zero vector */
  vector() {
    for(int32_t i=0; i<N; ++i) data_[i] = 0;
  }
  /** copy constructor */
  vector(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] = v.data_[i];
  }
  ~vector() {
  }

  inline T& operator()(int32_t i) {
    return data_[i];
  }
  inline const T& operator()(int32_t i) const {
    return data_[i];
  }
  inline T& operator[](int32_t i) {
    return (*this)(i);
  }
  inline const T& operator[](int32_t i) const {
    return (*this)(i);
  }

  inline vector operator+(const vector &v) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] + v.data_[i];
    return v2;
  }
  inline vector operator-(const vector &v) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] - v.data_[i];
    return v2;
  }
  inline vector operator*(const T &t) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] * t;
    return v2;
  }
  inline vector operator/(const T &t) const {
    T recip = static_cast<T>(1)/t;
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] * recip;
    return v2;
  }

  inline vector& operator+=(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] += v.data_[i];
    return *this;
  }
  inline vector& operator-=(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] -= v.data_[i];
    return *this;
  }
  inline vector& operator*=(const T &t) {
    for(int32_t i=0; i<N; ++i) data_[i] *= t;
    return *this;
  }
  inline vector& operator/=(const T &t) {
    T recip = static_cast<T>(1)/t;
    for(int32_t i=0; i<N; ++i) data_[i] /= t;
    return *this;
  }

  inline T norm2() const {
    return inner_prod(*this, *this);
  }
  inline vector& normalize() {
    (*this) /= sqrtf(norm2());
    return *this;
  }
  inline vector normalized() const {
    return (*this) / sqrtf(norm2());
  }

  /** conversion operator */
  template<int M, typename S>
  operator vector<M, S>() const {
    vector<M, S> v;
    for(int i=0; i<std::min(M, N); ++i) {
      v(i) = (*this)(i);
    }
    return v;
  }

  template<typename V>
  operator V() const {
    V v;
    v.resize(N);
    for(int i=0; i<N; ++i) v[i] = (*this)(i);
    return v;
  }

private:
  T data_[N];
};

typedef vector<2, float> vector2f;
typedef vector<3, float> vector3f;
typedef vector<4, float> vector4f;

template<int N, typename T>
inline T inner_prod(const vector<N, T> &v1, const vector<N, T> &v2) {
  T result = 0;
  for(int i=0; i<N; ++i) {
    result += v1(i) * v2(i);
  }
  return result;
}

template<typename T>
inline vector<2, T> vector2(const T &a, const T &b) {
  vector<2, T> v;
  v(0) = a;
  v(1) = b;
  return v;
}

template<typename T>
inline vector<3, T> vector3(const T &a, const T &b, const T &c) {
  vector<3, T> v;
  v(0) = a;
  v(1) = b;
  v(2) = c;
  return v;
}

template<typename T>
inline vector<4, T> vector4(const T &a, const T &b, const T &c, const T &d) {
  vector<4, T> v;
  v(0) = a;
  v(1) = b;
  v(2) = c;
  v(3) = d;
  return v;
}

template<int N, typename T>
std::ostream& operator<<(std::ostream &o, const vector<N,T> &v) {
  o << "(" << v(0);
  for(int i=1; i<N; ++i) {
    o << ", " << v(i);
  }
  o << ")";
  return o;
}

}

#endif /* GHP_MATH_VECTOR_HPP_ */

