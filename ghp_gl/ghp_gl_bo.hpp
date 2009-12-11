#ifndef _GHP_GL_BO_UTILS_HPP_
#define _GHP_GL_BO_UTILS_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <ghp_math.hpp>
#include <GL/gl.h>

namespace ghp {
namespace gl {

template<typename T> inline GLuint _gl_type_() 
  { return 0; }
template<> inline GLuint _gl_type_<float>() 
  { return GL_FLOAT; }
template<> inline GLuint _gl_type_<double>()
  { return GL_DOUBLE; }
template<> inline GLuint _gl_type_<int8_t>() 
  { return GL_BYTE; }
template<> inline GLuint _gl_type_<uint8_t>() 
  { return GL_UNSIGNED_BYTE; }
template<> inline GLuint _gl_type_<int16_t>() 
  { return GL_SHORT; }
template<> inline GLuint _gl_type_<uint16_t>()
  { return GL_UNSIGNED_SHORT; }
template<> inline GLuint _gl_type_<int32_t>()
  { return GL_INT; }
template<> inline GLuint _gl_type_<uint32_t>()
  { return GL_UNSIGNED_INT; }

enum bo_type : uint8_t {
  STATIC = 0,
  DYNAMIC = 1,
  STREAM = 2
};

enum bo_usage : uint8_t {
  READ = 0,
  COPY = 1,
  DRAW = 2
};

GLuint get_usage_code(bo_type type, bo_usage usage) {
  uint8_t switchCode = (type << 2) | usage;
  GLuint usage_code;
  switch(switchCode) {
    case (STATIC << 2) | READ: 
      usage_code = GL_STATIC_READ_ARB; break;
    case (STATIC << 2) | COPY:
      usage_code = GL_STATIC_COPY_ARB; break;
    case (STATIC << 2) | DRAW:
      usage_code = GL_STATIC_DRAW_ARB; break;
    case (DYNAMIC << 2) | READ:
      usage_code = GL_DYNAMIC_READ_ARB; break;
    case (DYNAMIC << 2) | COPY:
      usage_code = GL_DYNAMIC_COPY_ARB; break;
    case (DYNAMIC << 2) | DRAW:
      usage_code = GL_DYNAMIC_DRAW_ARB; break;
    case (STREAM << 2) | READ:
      usage_code = GL_STREAM_READ_ARB; break;
    case (STREAM << 2) | COPY:
      usage_code = GL_STREAM_COPY_ARB; break;
    case (STREAM << 2) | DRAW:
      usage_code = GL_STREAM_DRAW_ARB; break;
    default:
      std::cerr << "Invalid usage/type combination: "
        << "usage = " << usage 
        << ", type = " << type << std::endl;
      assert(0);
  }
  return usage_code;
}

template<int N, typename T, GLuint BUFFER_ARG>
class bo {
public:
  bo(bo_type type=STATIC, bo_usage usage=DRAW)
      : glSynced_(false),
      glBuffer_(-1),
      type_(type),
      usage_(usage) {
    set_usage_code_();
  }

  bo(const bo &b)
      : buffer_(b.buffer_),
      glSynced_(false),
      glBuffer_(-1),
      type_(b.type_),
      usage_(b.usage_),
      usage_code_(b.usage_code_) {
  }

  virtual ~bo() {
    if(glBuffer_ != static_cast<GLuint>(-1)) {
      glDeleteBuffersARB(1, &glBuffer_);
    }
  }

  bo& operator=(const bo &v) {
    glSynced_ = false;
    buffer_ = v.buffer_;
    type_ = v.type_;
    usage_ = v.usage_;
    usage_code_ = v.usage_code_;
    return *this;
  }

  const std::vector<math::vector<N, T> >& get_buffer() const {
    return buffer_;
  }

  std::vector<math::vector<N, T> >& mod_buffer() {
    glSynced_ = false;
    return buffer_;
  }

  bo_type get_type() const {
    return type_;
  }

  bo& set_type(bo_type t) {
    glSynced_ = false;
    type_ = t;
    set_usage_code_();
    return *this;
  }

  bo_usage get_usage() const {
    return usage_;
  }

  bo& set_usage(bo_usage u) {
    glSynced_ = false;
    usage_ = u;
    set_usage_code_();
    return *this;
  }

private:
  void set_usage_code_() {
    usage_code_ = get_usage_code(type_, usage_);
  }

protected:
  void sync_gl_() const {
    if(glSynced_) return;
    if(glBuffer_ == static_cast<GLuint>(-1)) {
      glGenBuffersARB(1, &glBuffer_);
    }
    glBindBufferARB(BUFFER_ARG, glBuffer_);
    int32_t size = sizeof(T) * N * buffer_.size();
    const uint8_t *data = reinterpret_cast<const uint8_t*>
      (&buffer_[0]);
    glBufferDataARB(BUFFER_ARG, size, data, usage_code_);
    glSynced_ = true;
  }

  inline GLuint get_gl_buffer_() const {
    return glBuffer_;
  }

  inline void invalidate_gl_() {
    glSynced_ = false;
  }

private:
  std::vector<math::vector<N, T> > buffer_;
  mutable bool glSynced_;
  mutable GLuint glBuffer_;
  bo_type type_;
  bo_usage usage_;
  GLuint usage_code_;
};

}
}

#endif

