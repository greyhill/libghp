#include "display.hpp"

#include <SDL/SDL.h>

#include <stdexcept>

namespace ghp {

//
// display impl
//
display::display(uint32_t w, uint32_t h, bool fs,
  const std::string &caption)
    : width_(w),
    height_(h),
    fullscreen_(fs),
    ratio_( float(w)/float(h) ),
    caption_(caption) {
  // start the display
  if(SDL_Init(SDL_INIT_VIDEO) == -1)
    throw std::runtime_error(SDL_GetError());
  if(SDL_SetVideoMode(width_, height_, 0, SDL_OPENGL | SDL_DOUBLEBUF
      | (fullscreen_ ? SDL_FULLSCREEN : 0)) == NULL)
    throw std::runtime_error(SDL_GetError());
  SDL_WM_SetCaption(caption_.c_str(), NULL);
}

display::~display() {
  SDL_Quit();
}

uint32_t display::get_width() const {
  return width_;
}

uint32_t display::get_height() const {
  return height_;
}

bool display::get_fullscreen() const {
  return fullscreen_;
}

float display::get_ratio() const {
  return ratio_;
}

const std::string& display::get_caption() const {
  return caption_;
}

display& display::set_caption(const std::string &s) {
  caption_ = s;
  SDL_WM_SetCaption(caption_.c_str(), NULL);
  return *this;
}

}

// eof //

