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
  glEnableClientState(GL_VERTEX_ARRAY);
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

template<GLenum TYPE, typename T>
class vbo : boost::noncopyable {
public:
  vbo() 
      : id_(-1) {
    glGenBuffersARB(1, &id_);
  }
  ~vbo() {
    if(id_ != -1) {
      glDeleteBuffersARB(1, &id_);
    }
  }

  inline GLuint id() const {
    return id_;
  }
  
  template<typename A>
  void write(const A &a, std::size_t size, 
      GLenum usage=GL_STATIC_DRAW) {
    CHECKED_GL_CALL(glBindBufferARB, (TYPE, id_));
    CHECKED_GL_CALL(glBufferDataARB, 
       (TYPE,
        size,
        &a[0],
        usage));
  }
  template<typename A>
  void update(const A &a, std::size_t size,
      std::size_t offset=0) {
    CHECKED_GL_CALL(glBindBufferARB, (TYPE, id_));
    CHECKED_GL_CALL(glBufferSubDataARB, 
       (TYPE,
        offset,
        size,
        &a[0]));
  }
private:
  GLuint id_;
};

template<int N, typename PIXELT>
class texture : boost::noncopyable {
public:
  texture();
  ~texture();

  template<typename PIXELT2>
  void write(const ghp::texture<PIXELT2> &tex) {
  }

private:
  GLuint id_;
};

#undef CHECKED_GL_CALL

}

#endif

