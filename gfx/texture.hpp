#ifndef _GHP_TEXTURE_HPP_
#define _GHP_TEXTURE_HPP_

#include "color.hpp"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include <string>

#include <GL/gl.h>

#include <stdint.h>

namespace ghp {

class texture {
public:
  texture();
  texture(const std::string &path);
  texture(uint32_t width, uint32_t height, uint8_t *rgba_pixels = NULL);
  texture(const texture &t);
  ~texture();

  uint32_t get_width() const;
  uint32_t get_height() const;
  GLuint get_texture_id() const;

  const color<uint8_t>& operator()(uint32_t r, uint32_t c) const;
  color<uint8_t>& operator()(uint32_t r, uint32_t c);

  texture& operator=(const texture &t);

private:
  boost::scoped_array<color<uint8_t> > pixels_;
  uint32_t width_;
  uint32_t height_;
  mutable bool synced_;
  mutable boost::scoped_ptr<class texture_gl> gltex_;
};

}

#endif

