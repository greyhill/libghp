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
  const uint8_t *pixel_ptr = static_cast<const uint8_t*>(surf->pixels);
  const uint8_t bpp = surf->format->BytesPerPixel;
  const uint32_t red_mask = surf->format->Rmask;
  const uint8_t red_shift = surf->format->Rshift;
  const uint32_t green_mask = surf->format->Gmask;
  const uint8_t green_shift = surf->format->Gshift;
  const uint32_t blue_mask = surf->format->Bmask;
  const uint8_t blue_shift = surf->format->Bshift;
  const uint32_t alpha_mask = surf->format->Amask;
  const uint8_t alpha_shift = surf->format->Ashift;
  for(uint32_t r=0; r < width_; ++r) {
    for(uint32_t c=0; c < height_; ++c) {
      pixel_ptr += bpp;
      const uint32_t *pixel_int = 
        reinterpret_cast<const uint32_t*>(pixel_ptr);
      pixels_[r*width_ + c].red() = 
        static_cast<uint8_t>((*pixel_int * red_mask) >> red_shift);
      pixels_[r*width_ + c].green() = 
        static_cast<uint8_t>((*pixel_int * green_mask) >> green_shift);
      pixels_[r*width_ + c].blue() = 
        static_cast<uint8_t>((*pixel_int * blue_mask) >> blue_shift);
      pixels_[r*width_ + c].alpha() = 
        static_cast<uint8_t>((*pixel_int * alpha_mask) >> alpha_shift);
    }
  }
  SDL_FreeSurface(surf);
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

GLuint texture::get_texture_id_() const {
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
  synced_ = false;
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

