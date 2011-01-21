#ifndef _GHP_GFX_GL_HPP_
#define _GHP_GFX_GL_HPP_

#include "gl_bitmap_font.hpp"

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

#ifndef NDEBUG
#define CHECK_GL { \
  GLenum gl_err_ = glGetError(); \
  if(gl_err_ != GL_NO_ERROR) { \
    throw std::runtime_error((char*)gluErrorString(gl_err_)); \
  } \
}; 
#else
#define CHECK_GL ;
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
  glEnable(GL_TEXTURE_2D);
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

template<typename PIXELT> struct pixelt2gl;
template<typename T> struct pixelt2gl<ghp::RGB<T> > {
  static const GLuint value = GL_RGB;
};
template<typename T> struct pixelt2gl<ghp::RGBA<T> > {
  static const GLuint value = GL_RGBA;
};
template<typename T> struct pixelt2gl<ghp::Single<0, ghp::RGBA<T> > > {
  static const GLuint value = GL_RED;
};
template<typename T> struct pixelt2gl<ghp::Single<1, ghp::RGBA<T> > > {
  static const GLuint value = GL_GREEN;
};
template<typename T> struct pixelt2gl<ghp::Single<2, ghp::RGBA<T> > > {
  static const GLuint value = GL_BLUE;
};
template<typename T> struct pixelt2gl<ghp::Single<3, ghp::RGBA<T> > > {
  static const GLuint value = GL_ALPHA;
};
template<typename T> struct pixelt2gl<ghp::Single<0, ghp::RGB<T> > > {
  static const GLuint value = GL_RED;
};
template<typename T> struct pixelt2gl<ghp::Single<1, ghp::RGB<T> > > {
  static const GLuint value = GL_GREEN;
};
template<typename T> struct pixelt2gl<ghp::Single<2, ghp::RGB<T> > > {
  static const GLuint value = GL_BLUE;
};

template<int N, typename PIXELT>
class texture { };

template<typename PIXELT>
class texture<2, PIXELT> : boost::noncopyable {
public:
  inline texture() 
      : id_(-1) {
    CHECKED_GL_CALL(glGenTextures, (1, &id_));
  }
  inline ~texture() {
    if(id_ != -1) {
      CHECKED_GL_CALL(glDeleteTextures, (1, &id_));
    }
  }

  template<typename PIXELT2>
  void write(const ghp::texture<PIXELT2> &tex,
      GLuint min_filter = GL_LINEAR,
      GLuint mag_filter = GL_LINEAR) {
    CHECKED_GL_CALL(glBindTexture, (GL_TEXTURE_2D, id_));
    CHECKED_GL_CALL(glTexParameteri, (GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER, min_filter));
    CHECKED_GL_CALL(glTexParameteri, (GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, mag_filter));
    CHECKED_GL_CALL(glTexImage2D, (GL_TEXTURE_2D,
        0,
        pixelt2gl<PIXELT>::value,
        tex.width(),
        tex.height(),
        0,
        pixelt2gl<PIXELT2>::value,
        cpp2gl<typename PIXELT2::value_type>::value,
        &tex[0]));
  }
  void resize(std::size_t width, std::size_t height,
      GLuint min_filter = GL_LINEAR,
      GLuint mag_filter = GL_LINEAR) {
    CHECKED_GL_CALL(glBindTexture, (GL_TEXTURE_2D, id_));
    CHECKED_GL_CALL(glTexParameteri, (GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER, min_filter));
    CHECKED_GL_CALL(glTexParameteri, (GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, mag_filter));
    CHECKED_GL_CALL(glTexImage2D, (GL_TEXTURE_2D,
        0,
        pixelt2gl<PIXELT>::value,
        width,
        height,
        0,
        pixelt2gl<PIXELT>::value,
        cpp2gl<typename PIXELT::value_type>::value,
        NULL));
  }

  inline GLuint id() const {
    return id_;
  }

private:
  GLuint id_;
};

template<GLuint type> 
class shader : boost::noncopyable {
public:
  inline shader(const std::string &source) 
      : id_(NULL) {
    id_ = glCreateShader(type);
    CHECK_GL;
    const char *src = source.c_str();
    CHECKED_GL_CALL(glShaderSource, (id_,
        1, &src, NULL));
    CHECKED_GL_CALL(glCompileShader, (id_));
  }
  inline ~shader() {
    if(id_ != NULL) {
      CHECKED_GL_CALL(glDeleteShader, (id_));
    }
  }

  inline GLuint id() const {
    return id_;
  }
private:
  GLuint id_;
};

template<int UNUSED=0>
class program_ : boost::noncopyable {
public:
  inline program_() 
      : id_(NULL) {
    id_ = glCreateProgram();
    CHECK_GL; 
  }
  inline ~program_() {
    if(id_ != NULL) {
      CHECKED_GL_CALL(glDeleteProgram, (id_));
    }
  }

  template<GLuint T>
  inline void attach(const shader<T> &sh) {
    CHECKED_GL_CALL(glAttachShader, (id_, sh.id()));
  }
  template<GLuint T>
  inline void detach(const shader<T> &sh) {
    CHECKED_GL_CALL(glDetachShader, (id_, sh.id()));
  }
  inline void link() {
    CHECKED_GL_CALL(glLinkProgram, (id_));
  }

  inline GLuint id() const {
    return id_;
  }
private:
  GLuint id_;
};
typedef program_<0> program;

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

template<int N> struct gltexdim { };
template<> struct gltexdim<1> {
  static const GLenum value = GL_TEXTURE_1D;
};
template<> struct gltexdim<2> {
  static const GLenum value = GL_TEXTURE_2D;
};
template<> struct gltexdim<3> {
  static const GLenum vlaue = GL_TEXTURE_3D;
};

template<int N, typename PIXELT>
inline void bind_texture(const texture<N, PIXELT> &tex) {
  CHECKED_GL_CALL(glBindTexture,
      (gltexdim<N>::value, tex.id()));
}

template<int N>
static inline void unbind_texture() {
  CHECKED_GL_CALL(glBindTexture, 
      (gltexdim<N>::value, 0));
}

static inline void bind_program(const program &p) {
  CHECKED_GL_CALL(glUseProgram, (p.id()));
}
static inline void unbind_program() {
  CHECKED_GL_CALL(glUseProgram, (0));
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
inline void tex_coord_pointer(
    size_t dim,
    size_t stride=0,
    const PTR &ptr=0) {
  CHECKED_GL_CALL(glTexCoordPointer,
      (dim,
      cpp2gl<T>::value,
      stride,
      reinterpret_cast<void*>(ptr)));
}

template<typename T, typename PTR>
inline void draw_elements(
    GLenum type,
    size_t nelem,
    const PTR &ptr=0) {
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

template<typename PIXELT>
inline void clear_color(const ghp::color<PIXELT> &c) {
  ghp::color<ghp::RGBA<float> > t = c;
  glClearColor(t.red(), t.green(), t.blue(), t.alpha());
}

#undef CHECKED_GL_CALL

}

#endif

