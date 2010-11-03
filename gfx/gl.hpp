#ifndef _GHP_GFX_GL_HPP_
#define _GHP_GFX_GL_HPP_

#include "color.hpp"

#include "../math.hpp"
#include "../util.hpp"

#include <glew.h>
#include <gl.h>
#include <glu.h>

#include <cassert>
#include <iostream>

namespace gl {

/* if NDEBUG (the C assert library's debug macro) is defined,
  error checking is disabled.  otherwise, throws exceptions
  when a GL error occurs */
#ifndef NDEBUG
#define CHECKED_GL_CALL(x, args) { \
  x args ; \
  GLenum gl_err_ = glGetError(); \
  if(gl_err_ != GL_NO_ERROR) { \
    throw std::runtime_error((char*)gluErrorString(gl_err_)); \
  } \
};
#else
#define CHECKED_GL_CALL(x, args) x args ;
#endif

/** \brief types of primitives that can be drawn by openGL */
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

/** \brief call this before using any other functions in this
  suite */
inline void init() {
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    std::cerr << "error initializing GLEW: " << glewGetErrorString(err)
        << std::endl;
  } else {
    std::clog << "glew initialized, version " << glewGetString(GLEW_VERSION)
        << std::endl;
  }
}

/** \brief traits for converting between C++ and OpenGL types */
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

/** \brief type traits for retreiving OpenGL color format based on the
  number of bytes per pixel.  OpenGL supports only RGB and RGBA colors in
  the fixed-function pipeline. */
template<int N, typename T> struct gl_color_from_num { };
template<typename T> struct gl_color_from_num<3, T> {
  typedef ghp::color<ghp::RGB<T> > value_type;
};
template<typename T> struct gl_color_from_num<4, T> {
  typedef ghp::color<ghp::RGBA<T> > value_type;
};

/** \brief special pointer for client-side buffers of vertex data.
  supports the vertex buffer concept.
  \tparam N - dimension: 2, 3 or 4
  \tparam T - type: float, double or short (int16_t)
  \tparam PTR - a pointer or smart-pointer for storing the data.
    specializations of ghp::generic_ptr_deref may be necessary
    to use smart pointers other than boost::shared_ptr and 
    boost::shared_array.  default is T*
 */
template<int N, typename T, typename PTR=T*> class vertex_ptr {
public:
  typedef ghp::vector<N, T> value_type;
  
  /** \brief create a new vertex_ptr.
    \param a - indexable type that stores vertex data contiguously
      in memory
    \param stride - extra space -- in bytes -- between adjacent 
      vertex elements.  default is 0, meaning dense packing. */
  template<typename A> 
  inline vertex_ptr(A a, std::size_t stride=0) 
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  /** \brief trivial dtor */
  inline ~vertex_ptr() { }
  /** \brief set this vertex buffer as active.  this function
    modifies OpenGL state, so is not threadsafe */
  inline void activate() {
    assert(data_ != NULL);
    CHECKED_GL_CALL(glEnableClientState, (GL_VERTEX_ARRAY));
    CHECKED_GL_CALL(glVertexPointer, 
       (N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_)));
  }
  /** \brief unsets this vertex buffer */
  inline void deactivate() {
    CHECKED_GL_CALL(glDisableClientState, (GL_VERTEX_ARRAY));
  }
  /** \brief reset the vertex_ptr's data pointer */
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  /** \brief change the vertex_ptr's stride */
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  /** \brief element access */
  inline const ghp::vector<N, T>& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  /** \brief element access */
  inline ghp::vector<N, T>& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<N, T>*>(base+N*i);
  }
  /** \brief element access */
  inline const ghp::vector<N, T>& operator[](int i) const {
    return (*this)(i);
  }
  /** \brief element access */
  inline ghp::vector<N, T>& operator[](int i) {
    return (*this)(i);
  }
private:
  PTR data_;
  std::size_t stride_;
};

/** \brief special pointer for client-side buffers of normal
  data.  supports the normal buffer concept.
  \tparam T - type: float, double or short (int16_t)
  \tparam PTR - a pointer or smart-pointer for storing the data.
    specializations of ghp::generic_ptr_deref may be necessary
    to use smart pointers other than boost::shared_ptr and 
    boost::shared_array.  default is T*
 */
template<typename T, typename PTR=T*> class normal_ptr {
public:
  typedef ghp::vector<3, T> value_type;

  /** \brief create a new normal_ptr.
    \param a - indexable type that stores vertex data contiguously
      in memory
    \param stride - extra space -- in bytes -- between adjacent 
      vertex elements.  default is 0, meaning dense packing. */
  template<typename A>
  inline normal_ptr(A a, std::size_t stride=0)
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  /** \brief trivial dtor */
  inline ~normal_ptr() { }
  /** \brief set this normal buffer as active.  this function
    modifies OpenGL state, so is not threadsafe */
  inline void activate() {
    assert(data_ != NULL);
    CHECKED_GL_CALL(glEnableClientState, (GL_NORMAL_ARRAY));
    CHECKED_GL_CALL(glNormalPointer, 
       (cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_)));
  }
  /** \brief unsets this normal buffer */
  inline void deactivate() {
    CHECKED_GL_CALL(glDisableClientState, (GL_NORMAL_ARRAY));
  }
  /** \brief reset (change) the buffer's data pointer */
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  /** \brief change the normal_ptr's stride */
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  /** \brief element access */
  inline const ghp::vector<3, T>& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<3, T>*>(base+3*i);
  }
  /** \brief element access */
  inline ghp::vector<3, T>& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<ghp::vector<3, T>*>(base+3*i);
  }
  /** \brief element access */
  inline const ghp::vector<3, T>& operator[](int i) const {
    return (*this)(i);
  }
  /** \brief element access */
  inline ghp::vector<3, T>& operator[](int i) {
    return (*this)(i);
  }
private:
  PTR data_;
  std::size_t stride_;
};

/** \brief special pointer for client-side buffers of color
  data.  supports the color buffer concept.
  \tparam N - number of color coordinates: either 3 (RGB) or 4 (RGBA)
  \tparam T - underlying type: uint8_t, int8_t, uint16_t, int16_t, 
    uint32_t, int32_t, float or double
  \tparam PTR - pointer or smart pointer type for storing the data.
    specializations of ghp::generic_ptr_deref may be necessary
    to use smart pointers other than boost::shared_ptr and
    boost::shared_array.  default is T* 
 */
template<int N, typename T, typename PTR=T*> class color_ptr {
public:
  typedef typename gl_color_from_num<N, T>::value_type value_type;

  /** \brief create a new color_ptr
    \param a - indexable type that stores color data contiguously
      in memory
    \param stride - extra space -- in bytes -- between adjacent
      color elements.  default is 0, meaning dense packing */
  template<typename A>
  inline color_ptr(A a, std::size_t stride=0)
      : data_(reinterpret_cast<T*>(&a[0])),
      stride_(stride) {
  }
  /** \brief trivial destructor */
  inline ~color_ptr() { }
  /** \brief set this color buffer as active.  this function
    modifies OpenGL state, so it is not threadsafe */
  inline void activate() {
    assert(data_ != NULL);
    CHECKED_GL_CALL(glEnableClientState, (GL_COLOR_ARRAY));
    CHECKED_GL_CALL(glNormalPointer, 
       (N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_)));
  }
  /** \brief unsets this vertex buffer.  this function modifies
    OpenGL state, so it is not threadsafe */
  inline void deactivate() {
    CHECKED_GL_CALL(glDisableClientState, (GL_VERTEX_ARRAY));
  }
  /** \brief reset the vertex_ptr's data pointer */
  template<typename A> inline void reset(A a) {
    data_ = reinterpret_cast<T*>(&a[0]);
  }
  /** \brief change the vertex_ptr's stride */
  inline void set_stride(std::size_t stride) {
    stride_ = stride;
  }
  /** \brief element access */
  inline const value_type& operator()(int i) const {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<value_type*>(base+N*i);
  }
  /** \brief element access */
  inline value_type& operator()(int i) {
    T *base = ghp::generic_ptr_deref<T, PTR>()(data_);
    return *reinterpret_cast<value_type*>(base+N*i);
  }
  /** \brief element access */
  inline const value_type& operator[](int i) const {
    return (*this)[i];
  }
  /** \brief element access */
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
    CHECKED_GL_CALL(glEnableClientState, (GL_TEXTURE_COORD_ARRAY));
    CHECKED_GL_CALL(glTexCoordPointer, 
       (N,
        cpp2gl<T>::value,
        stride_,
        ghp::generic_ptr_deref<T, PTR>()(data_)));
  }
  inline void deactivate() {
    CHECKED_GL_CALL(glDisableClientState, (GL_TEXTURE_COORD_ARRAY));
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

#undef CHECKED_GL_CALL

}

#endif

