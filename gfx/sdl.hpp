#ifndef _GHP_GFX_SDL_HPP_
#define _GHP_GFX_SDL_HPP_

#include "color.hpp"
#include "texture.hpp"

#include "../math.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>
#include <string>

#include <cassert>

namespace sdl {

/**
  \brief load a texture from disk using SDL_image
  \tparam PIXELT - desired target pixel type
  \param path - path of texture to load
  \param dest - where to place the texture
 */
template<typename PIXELT>
void load_texture(const std::string &path, ghp::texture<PIXELT> &dest) {
  SDL_Surface *surf = IMG_Load(path.c_str());
  SDL_LockSurface(surf);
  if(surf == NULL) {
    throw std::runtime_error(IMG_GetError());
  }
  dest.resize(surf->w, surf->h);
  const int num_pixels = dest.get_width() * dest.get_height();
  const uint8_t *pixels = reinterpret_cast<const uint8_t*>(surf->pixels);
  if(surf->format->BytesPerPixel == 3) {
    for(int pix_id = 0; pix_id < num_pixels; ++pix_id) {
      ghp::color<ghp::RGB<typename PIXELT::value_t> > pixel;
      const uint32_t *src_pixel = reinterpret_cast<const uint32_t*>(
          pixels + 3*pix_id);
      uint8_t r, g, b;
      SDL_GetRGB(
        *src_pixel,
        surf->format,
        &r, &g, &b
      );
      pixel.red() = r; pixel.green() = g; pixel.blue() = b;
      dest(pix_id) = pixel;
    }
  } else if(surf->format->BytesPerPixel == 4) {
    for(int pix_id = 0; pix_id < num_pixels; ++pix_id) {
      ghp::color<ghp::RGBA<typename PIXELT::value_t> > pixel;
      const uint32_t *src_pixel = reinterpret_cast<const uint32_t*>(
          pixels + 4*pix_id);
      uint8_t r, g, b, a;
      SDL_GetRGBA(
        *src_pixel,
        surf->format,
        &r, &g, &b, &a
      );
      pixel.red() = r; pixel.green() = g; pixel.blue() = b; pixel.alpha() = a;
      dest(pix_id) = pixel;
    }
  } else {
    SDL_UnlockSurface(surf);
    SDL_FreeSurface(surf);
    throw std::runtime_error("can only load 3 or 4 BytesPerPixel images");
  }
  SDL_UnlockSurface(surf);
  SDL_FreeSurface(surf);
}

/**
  \brief save a texture to disk as a BMP
  \tparam PIXELT - pixel type
  \param path - path to save to
  \param src - texture to save
 */
template<typename PIXELT>
void save_bmp(const std::string &path, const ghp::texture<PIXELT> &src) {
  SDL_Surface *surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 
      src.get_width(), src.get_height(), 
      24,
      0xFF0000,
      0x00FF00,
      0x0000FF,
      0x000000);
  if(surf == NULL) throw std::runtime_error(SDL_GetError());
  SDL_LockSurface(surf);
  const int num_pixels = src.get_width() * src.get_height();
  uint8_t *dest_pixels = reinterpret_cast<uint8_t*>(surf->pixels);
  for(int i=0; i<num_pixels; ++i) {
    uint32_t *this_pixel = reinterpret_cast<uint32_t*>(
        dest_pixels + 3*i);
    ghp::color<ghp::RGB<uint8_t> > convert_color = src(i);
    *this_pixel = SDL_MapRGB(surf->format,
        convert_color.red(),
        convert_color.green(),
        convert_color.blue()
    );
  }
  SDL_SaveBMP(surf, path.c_str());
  SDL_UnlockSurface(surf);
  SDL_FreeSurface(surf);
}

/**
  \brief software display window from SDL.  The soft-display supports
  the texture concept.
  \tparam PIXELT - underlying pixel type
 */
template<typename PIXELT>
class soft_display {
public:
  typedef PIXELT pixel_t;

  /**
    \brief create a new software display window.  The window
    appears once the constructor completes, and disappears
    when this object is destroyed.
    \param width - window width
    \param height - window height
    \param fullscreen - make the window fullscreen?
   */
  soft_display(int32_t width, int32_t height, bool fullscreen)
      : width_(width),
      height_(height),
      fullscreen_(fullscreen),
      screen_(NULL),
      texture_(width, height) {
    init_();
  }
  ~soft_display() {
    SDL_Quit();
  }

  /**
    \brief set the window caption
   */
  inline void set_caption(const std::string &s) {
    SDL_WM_SetCaption(s.c_str(), NULL);
  }
  /**
    \brief update the window.  must be called to update the display
   */
  void update() {
    // copy pixels from internal texture to SDL surface (screen memory)
    const int num_pixels = width_ * height_;
    uint8_t *pixels = reinterpret_cast<uint8_t*>(screen_->pixels);
    const int bpp = screen_->format->BytesPerPixel;
    for(int i=0; i<num_pixels; ++i) {
      uint32_t *this_pixel = reinterpret_cast<uint32_t*>(pixels + bpp*i);
      ghp::color<ghp::RGB<uint8_t> > tmp_color = texture_(i);
      *this_pixel = SDL_MapRGB(screen_->format,
        tmp_color.red(), tmp_color.blue(), tmp_color.green());
    }
    // ...aaand update
    SDL_Flip(screen_);
  }

  /** \brief pixel access. */
  inline const ghp::color<PIXELT>& operator()(int x, int y) const {
    return texture_(x, y);
  }
  /** \brief pixel access. */
  inline ghp::color<PIXELT>& operator()(int x, int y) {
    return texture_(x, y);
  }
  /** \brief pixel access. */
  inline const ghp::color<PIXELT>& operator()
      (const ghp::vector<2, int> &v) const {
    return texture_(v(0), v(1));
  }
  /** \brief pixel access. */
  inline ghp::color<PIXELT>& operator()
      (const ghp::vector<2, int> &v) {
    return texture_(v(0), v(1));
  }
  /** \brief gets the screen width */
  inline int get_width() const {
    return width_;
  }
  /** \brief gets the screen height */
  inline int get_height() const {
    return height_;
  }

private:
  inline void init_() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
      throw std::runtime_error(SDL_GetError());
    }
    screen_ = SDL_SetVideoMode(width_, height_, 0, SDL_DOUBLEBUF |
        (fullscreen_ ? SDL_FULLSCREEN : 0));
    if(screen_ == NULL) {
      throw std::runtime_error(SDL_GetError());
    }
  }

  int32_t width_;
  int32_t height_;
  bool fullscreen_;
  SDL_Surface *screen_;
  
  ghp::texture<PIXELT> texture_;
};

/**
  \brief an opengl-enabled screen from SDL
 */
class gl_display {
public:
  gl_display(int width, int height, bool fs)
      : width_(width),
      height_(height),
      fullscreen_(fs) {
    init_();
  }
  ~gl_display() {
    SDL_Quit();
  }

  /** \brief set the window caption */
  inline void set_caption(const std::string &s) {
    SDL_WM_SetCaption(s.c_str(), NULL);
  }
  /** \brief update the window */
  inline void update() {
    SDL_GL_SwapBuffers();
  }

  /** \brief gets the screen width */
  inline int get_width() const {
    return width_;
  }
  /** \brief gets the screen height */
  inline int get_height() const {
    return height_;
  }

private:
  inline void init_() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
      throw std::runtime_error(SDL_GetError());
    }
    SDL_Surface *screen_ = SDL_SetVideoMode(width_, height_, 0, 
        SDL_DOUBLEBUF | (fullscreen_ ? SDL_FULLSCREEN : 0));
    if(screen_ == NULL) {
      throw std::runtime_error(SDL_GetError());
    }
  }

  int width_;
  int height_;
  bool fullscreen_;
};

}

#endif

