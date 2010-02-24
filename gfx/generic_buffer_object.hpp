#ifndef _GHP_GFX_GENERIC_BUFFER_OBJECT_HPP_
#define _GHP_GFX_GENERIC_BUFFER_OBJECT_HPP_

#include <boost/scoped_ptr.hpp>

#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

namespace ghp {

template<typename T, GLuint BUFFER_ARG>
class generic_buffer_object {
public:
  enum mutability {
    STATIC = 1,
    DYNAMIC = 2,
    STREAM = 4
  };

  enum use {
    READ = 1,
    COPY = 2,
    DRAW = 4
  };

  generic_buffer_object()
      : mutability_(STATIC),
      use_(DRAW),
      synced_(false),
      buffer_id_(static_cast<GLuint>(-1)) {
    set_mu_code_();
  }
  generic_buffer_object(const generic_buffer_object &obj) 
      : synced_(false),
      buffer_id_(static_cast<GLuint>(-1)) {
    *this = obj;
  }
  template<typename PTR>
  generic_buffer_object(std::size_t size, PTR ptr) 
      : mutability_(STATIC),
      use_(DRAW),
      buffer_(size),
      synced_(false),
      buffer_id_(static_cast<GLuint>(-1)) {
    for(std::size_t i=0; i<size; ++i) buffer_[i] = ptr[i];
  }
  virtual ~generic_buffer_object() {
    if(buffer_id_ != static_cast<GLuint>(-1)) {
      glDeleteBuffersARB(1, &buffer_id_);
    }
  }

  mutability get_mutability() const {
    return mutability_;
  }
  generic_buffer_object& set_mutability(mutability m) {
    synced_ = false;
    mutability_ = m;
    set_mu_code_();
    return *this;
  }
  use get_use() const { return use_; }
  generic_buffer_object& set_use(use u) {
    synced_ = false;
    use_ = u;
    set_mu_code_();
    return *this;
  }

  inline const T& operator()(uint32_t i) const { return buffer_[i]; }
  inline const T& operator[](uint32_t i) const { return (*this)(i); }
  inline T& operator()(uint32_t i) {
    synced_ = false;
    return buffer_[i];
  }
  inline T& operator[](uint32_t i) { return (*this)(i); }

  std::size_t size() const {
    return buffer_.size();
  }
  generic_buffer_object& resize(std::size_t s) {
    synced_ = false;
    buffer_.resize(s);
    return *this;
  }

  generic_buffer_object& operator=(const generic_buffer_object &b) {
    synced_ = false;
    mutability_ = b.mutability_;
    use_ = b.use_;
    buffer_ = b.buffer_;
  }

protected:
  GLuint get_buffer_id_() const {
    if(!synced_) {
      if(buffer_id_ == static_cast<GLuint>(-1)) {
        glGenBuffersARB(1, &buffer_id_);
      }
      glBindBufferARB(BUFFER_ARG, buffer_id_);
      std::size_t size = sizeof(T) * buffer_.size();
      const uint8_t *data = reinterpret_cast<const uint8_t*>
        (&buffer_[0]);
      glBufferDataARB(BUFFER_ARG, size, data, cached_mu_code_);
      synced_ = true;
    }
    return buffer_id_;
  }

private:
  void set_mu_code_() {
    const uint32_t switch_code = (mutability_ << 2) | use_;
    switch(switch_code) {
      case (STATIC << 2) | READ:
        cached_mu_code_ = GL_STATIC_READ_ARB; break;
      case (STATIC << 2) | COPY:
        cached_mu_code_ = GL_STATIC_COPY_ARB; break;
      case (STATIC << 2) | DRAW:
        cached_mu_code_ = GL_STATIC_DRAW_ARB; break;
      case (DYNAMIC << 2) | READ:
        cached_mu_code_ = GL_DYNAMIC_READ_ARB; break;
      case (DYNAMIC << 2) | COPY:
        cached_mu_code_ = GL_DYNAMIC_COPY_ARB; break;
      case (DYNAMIC << 2) | DRAW:
        cached_mu_code_ = GL_DYNAMIC_DRAW_ARB; break;
      case (STREAM << 2) | READ:
        cached_mu_code_ = GL_STREAM_READ_ARB; break;
      case (STREAM << 2) | COPY:
        cached_mu_code_ = GL_STREAM_COPY_ARB; break;
      case (STREAM << 2) | DRAW:
        cached_mu_code_ = GL_STREAM_DRAW_ARB; break;
      default:
        assert(-1);
    }
  }

  mutability mutability_;
  use use_;
  GLuint cached_mu_code_;
  std::vector<T> buffer_;

  mutable bool synced_;
  mutable GLuint buffer_id_;

friend class renderer;
};

}

#endif

