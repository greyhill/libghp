#ifndef _GHP_GFX_GL_HPP_
#define _GHP_GFX_GL_HPP_

#include "color.hpp"
#include "texture.hpp"

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

template<GLenum TYPE>
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
  texture() 
      : id_(-1) {
    CHECKED_GL_CALL(glGenTextures, 
        (1, &id_));
  }
  ~texture();

  template<typename PIXELT2>
  void write(const ghp::texture<PIXELT2> &tex) {
  }

private:
  GLuint id_;
};


namespace {
  template<int N, typename T> struct translate_fctor { };

  template<> struct translate_fctor<2, float> {
    inline void operator()(const ghp::vector<2, float> &v) {
      glTranslatef(v[0], v[1], 0);
    }
  };

  template<> struct translate_fctor<3, float> {
    inline void operator()(const ghp::vector<3, float> &v) {
      glTranslatef(v[0], v[1], v[2]);
    }
  };

  template<> struct translate_fctor<2, double> {
    inline void operator()(const ghp::vector<2, double> &v) {
      glTranslated(v[0], v[1], 0);
    }
  };

  template<> struct translate_fctor<3, double> {
    inline void operator()(const ghp::vector<3, double> &v) {
      glTranslated(v[0], v[1], v[2]);
    }
  };
}

template<int N, typename T>
inline void translate(const ghp::vector<N, T> &v) {
  translate_fctor<N, T> fct;
  fct(v);
}

template<GLenum TYPE>
inline void bind_buffer(const vbo<TYPE> &vbo) {
  CHECKED_GL_CALL(glBindBufferARB,
      (TYPE, vbo.id()));
}

template<GLenum TYPE>
inline void unbind_buffer() {
  CHECKED_GL_CALL(glBindBufferARB,
      (TYPE, 0));
}

template<typename T, typename PTR>
inline void vertex_pointer(
    size_t dim,
    size_t stride=0,
    const PTR &ptr=0) {
  CHECKED_GL_CALL(glVertexPointer,
      (dim, cpp2gl<T>::value, stride,
      reinterpret_cast<void*>(ptr)));
}

template<typename T, typename PTR>
inline void normal_pointer(
    size_t stride=0,
    const PTR &ptr=0) {
  CHECKED_GL_CALL(glNormalPointer,
      (cpp2gl<T>::value,
      stride,
      reinterpret_cast<void*>(ptr)));
}

template<typename T, typename PTR>
inline void color_pointer(
    size_t dim,
    size_t stride=0,
    const PTR &ptr=0) {
  CHECKED_GL_CALL(glColorPointer,
      (dim,
      cpp2gl<T>::value,
      stride,
      reinterpret_cast<void*>(ptr)));
}

template<typename T, typename PTR>
inline void draw_elements(
    GLenum type,
    size_t nelem,
    const PTR &ptr) {
  CHECKED_GL_CALL(glDrawElements,
      (type,
      nelem,
      cpp2gl<T>::value,
      reinterpret_cast<void*>(ptr)));
}

namespace {
  template<typename T> inline void mult_matrix_(const T *t) { }
  template<> inline void mult_matrix_<float>(const float *f) {
    glMultMatrixf(f);
  }
  template<> inline void mult_matrix_<double>(const double *d) {
    glMultMatrixd(d);
  }
}

template<typename T> struct rotate_fctor { };

template<int N, typename T>
struct rotate_fctor<ghp::rot_matrix<N, T> > {
  inline void operator()(const ghp::rot_matrix<N, T> &m) {
    T tmp[16];
    for(int r=0; r<4; ++r) {
      for(int c=0; c<4; ++c) {
        if( (r<N) && (c<N) ) {
          tmp[c*4+r] = m(r,c);
        } else {
          tmp[c*4+r] = r==c ? 1 : 0;
        }
      }
    }
    mult_matrix_<T>(tmp);
  }
};

template<typename T>
inline void rotate(const T &t) {
  rotate_fctor<T> fctor;
  fctor(t);
}

#undef CHECKED_GL_CALL

}

#endif

