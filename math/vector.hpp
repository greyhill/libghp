#ifndef GHP_MATH_VECTOR_HPP_
#define GHP_MATH_VECTOR_HPP_

#include <stdint.h>

namespace ghp { 

template<int N, typename T>
class vector {
public:
  vector() {
    for(int32_t i=0; i<N; ++i) data_[i] = 0;
  }
  vector(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] = v.data_[i];
  }
  ~vector() {
  }

  T& operator()(int32_t i) {
    return data_[i];
  }
  const T& operator()(int32_t i) const {
    return data_[i];
  }
  T& get(int32_t i) {
    return data_[i];
  }
  const T& get(int32_t i) const {
    return data_[i];
  }

  vector operator+(const vector &v) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] + v.data_[i];
    return v2;
  }
  vector operator-(const vector &v) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] - v.data_[i];
    return v2;
  }
  vector operator*(const T &t) const {
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] * t;
    return v2;
  }
  vector operator/(const T &t) const {
    T recip = static_cast<T>(1)/t;
    vector v2;
    for(int32_t i=0; i<N; ++i) v2.data_[i] = data_[i] * recip;
    return v2;
  }

  vector& operator+=(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] += v.data_[i];
    return *this;
  }
  vector& operator-=(const vector &v) {
    for(int32_t i=0; i<N; ++i) data_[i] -= v.data_[i];
    return *this;
  }
  vector& operator*=(const T &t) {
    for(int32_t i=0; i<N; ++i) data_[i] *= t;
    return *this;
  }
  vector& operator/=(const T &t) {
    T recip = static_cast<T>(1)/t;
    for(int32_t i=0; i<N; ++i) data_[i] /= t;
    return *this;
  }

private:
  T data_[N];
};

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

}

#endif /* GHP_MATH_VECTOR_HPP_ */

