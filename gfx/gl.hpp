#ifndef _GHP_GFX_GL_HPP_
#define _GHP_GFX_GL_HPP_

#include "color.hpp"

#include "../math.hpp"
#include "../util.hpp"

#include <gl.h>

#include <cassert>

namespace gl {

enum primitive_type {
  POINTS,
  LINES,
  LINE_STRIP,
  LINE_LOOP,
  TRIANGLES,
  TRIANGLE_STRIP,
  TRIANGLE_FAN,
  QUADS,
  QUAD_STRIP,
  POLYGON
};

template<typename T, int N=0> struct cpp2gl { };
template<int N> struct cpp2gl<int8_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<int8_t, N>::value = GL_BYTE;

template<int N> struct cpp2gl<uint8_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<uint8_t, N>::value = GL_UNSIGNED_BYTE;

template<int N> struct cpp2gl<int16_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<int16_t, N>::value = GL_SHORT;

template<int N> struct cpp2gl<uint16_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<uint16_t, N>::value = GL_UNSIGNED_SHORT;

template<int N> struct cpp2gl<int32_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<int32_t, N>::value = GL_INT;

template<int N> struct cpp2gl<uint32_t, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<uint32_t, N>::value = GL_UNSIGNED_INT;

template<int N> struct cpp2gl<float, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<float, N>::value = GL_FLOAT;

template<int N> struct cpp2gl<double, N> { static const GLenum value; };
template<int N> const GLenum cpp2gl<double, N>::value = GL_DOUBLE;

template<int N, typename T> struct gl_color_from_num { };
template<typename T> struct gl_color_from_num<3, T> {
  typedef ghp::color<ghp::RGB<T> > value_type;
};
template<typename T> struct gl_color_from_num<4, T> {
  typedef ghp::color<ghp::RGBA<T> > value_type;
};

template<int N, typename T, typename PTR=T*> class vertex_ptr {
public:
  typedef ghp::vector<N, T> value_type;
  
  template<typename A> 
  inline vertex_ptr(A a, std::size_t stride=0) 
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  inline ~vertex_ptr() { }
  inline void activate() {
    assert(data_ != NULL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(
        N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_) );
  }
  inline void deactivate() {
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  inline const ghp::vector<N, T>& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  inline ghp::vector<N, T>& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  inline const ghp::vector<N, T>& operator[](int i) const {
    return (*this)(i);
  }
  inline ghp::vector<N, T>& operator[](int i) {
    return (*this)(i);
  }
private:
  PTR data_;
  std::size_t stride_;
};

template<typename T, typename PTR=T*> class normal_ptr {
public:
  typedef ghp::vector<3, T> value_type;

  template<typename A>
  inline normal_ptr(A a, std::size_t stride=0)
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  inline ~normal_ptr() { }
  inline void activate() {
    assert(data_ != NULL);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_));
  }
  inline void deactivate() {
    glDisableClientState(GL_NORMAL_ARRAY);
  }
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  inline const ghp::vector<3, T>& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<3, T>*>(base+3*i);
  }
  inline ghp::vector<3, T>& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<3, T>*>(base+3*i);
  }
  inline const ghp::vector<3, T>& operator[](int i) const {
    return (*this)(i);
  }
  inline ghp::vector<3, T>& operator[](int i) {
    return (*this)(i);
  }
private:
  PTR data_;
  std::size_t stride_;
};

template<int N, typename T, typename PTR=T*> class color_ptr {
public:
  typedef typename gl_color_from_num<N, T>::value_type value_type;

  template<typename A>
  inline color_ptr(A a, std::size_t stride=0)
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  inline ~color_ptr() { }
  inline void activate() {
    assert(data_ != NULL);
    glEnableClientState(GL_COLOR_ARRAY);
    glNormalPointer(
        N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_));
  }
  inline void deactivate() {
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  inline const value_type& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<value_type*>(base+N*i);
  }
  inline value_type& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<value_type*>(base+N*i);
  }
  inline const value_type& operator[](int i) const {
    return (*this)[i];
  }
  inline value_type& operator[](int i) {
    return (*this)[i];
  }
private:
  PTR data_;
  std::size_t stride_;
};

template<int N, typename T, typename PTR=T*> class tex_coord_ptr {
public:
  template<typename A>
  inline tex_coord_ptr(A a, std::size_t stride=0) 
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  inline ~tex_coord_ptr() { }
  inline void activate() {
    assert(data_ != NULL);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(
        N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_));
  }
  inline void deactivate() {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  inline const ghp::vector<N, T>& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  inline ghp::vector<N, T>& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  inline const ghp::vector<N, T>& operator[](int i) const {
    return (*this)(i);
  }
  inline ghp::vector<N, T>& operator[](int i) {
    return (*this)(i);
  }
private:
  PTR data_;
  std::size_t stride_;
};

}

#endif

