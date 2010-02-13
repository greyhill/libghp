#include "texture.hpp"

#include <boost/noncopyable.hpp>

#include <stdexcept>

#include <GL/gl.h>
#include <SDL/SDL_image.h>

namespace ghp {

//
// texture_gl definition and implementation
//
class texture_gl : boost::noncopyable {
public:
  texture_gl(const texture &t) {
    glGenTextures(1, &id_);
    if(!glIsTexture(id_))
      throw std::runtime_error("error allocating gl texture");
    reset(t);
  }

  ~texture_gl() {
    glDeleteTextures(1, &id_);
  }

  void reset(const texture &t) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      t.get_width(),
      t.get_height(),
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      &t(0,0)
    );
  }

  GLuint get_texture_id() const {
    return id_;
  }

private:
  GLuint id_;
};

//
// texture implementation
//
texture::texture()
    : pixels_(NULL),
    width_(0),
    height_(0),
    synced_(false), 
    gltex_(NULL) {
}

texture::texture(const std::string &path) {
  SDL_Surface *surf = IMG_Load(path.c_str());
  if(surf == NULL) 
    throw std::runtime_error("error loading image from file");
  width_ = surf->w;
  height_ = surf->h;
  pixels_.reset(new color<uint8_t>[width_*height_]);
}

texture::texture(uint32_t width, uint32_t height, uint8_t *pixels) 
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

texture::texture(const texture &t) 
    : pixels_(new color<uint8_t>[t.width_ * t.height_]), 
    width_(t.width_),
    height_(t.height_),
    synced_(false),
    gltex_(NULL) {
  for(uint32_t i=0; i<width_*height_; ++i)
    pixels_[i] = t.pixels_[i];
}

texture::~texture() {
}

uint32_t texture::get_width() const {
  return width_;
}

uint32_t texture::get_height() const {
  return height_;
}

GLuint texture::get_texture_id() const {
  if(gltex_ == NULL) {
    gltex_.reset(new texture_gl(*this));
    synced_ = true;
    return gltex_->get_texture_id();
  } else if(!synced_) {
    gltex_->reset(*this);
    synced_ = true;
  }
  return gltex_->get_texture_id();
}

const color<uint8_t>& texture::operator()(uint32_t r, uint32_t c) const {
  return pixels_[r*width_ + c];
}

color<uint8_t>& texture::operator()(uint32_t r, uint32_t c) {
  return pixels_[r*width_ + c];
}

texture& texture::operator=(const texture &t) {
  if((width_*height_) != (t.width_ * t.height_))
    pixels_.reset(new color<uint8_t>[t.width_ * t.height_]);
  width_ = t.width_;
  height_ = t.height_;
  synced_ = false;
  for(uint32_t i=0; i<width_*height_; ++i)
    pixels_[i] = t.pixels_[i];
  return *this;
}

}

// eof //

