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
  texture(uint32_t width, uint32_t height);
  texture(const texture &t);

  template<typename PTR>
  texture(uint32_t width, uint32_t height, PTR pixels) 
      : pixels_(new color<uint8_t>[width*height]),
      width_(width),
      height_(height),
      synced_(false) {
    for(uint32_t i=0; i<width*height; ++i) {
      pixels_[i].red() = pixels[4*i];
      pixels_[i].green() = pixels[4*i + 1];
      pixels_[i].blue() = pixels[4*i + 2];
      pixels_[i].alpha() = pixels[4*i + 3];
    }
  }
  ~texture();

  uint32_t get_width() const;
  uint32_t get_height() const;

  const color<uint8_t>& operator()(uint32_t r, uint32_t c) const;
  color<uint8_t>& operator()(uint32_t r, uint32_t c);

  texture& operator=(const texture &t);

private:
  GLuint get_texture_id_() const;

  boost::scoped_array<color<uint8_t> > pixels_;
  uint32_t width_;
  uint32_t height_;
  mutable bool synced_;
  mutable boost::scoped_ptr<class texture_gl> gltex_;

friend class renderer;
};

}

#endif

