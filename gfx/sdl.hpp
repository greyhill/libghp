#ifndef _GHP_GFX_SDL_HPP_
#define _GHP_GFX_SDL_HPP_

#include "color.hpp"
#include "font.hpp"
#include "texture.hpp"

#include "../math.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <string>

#include <cassert>
#include <cmath>

namespace sdl {

/**
  \brief convert from an SDL_Surface to a ghp texture
  \tparam TEX - has texture concept
  \param surf - source surface
  \param dest - destination texture
 */
template<typename TEX>
void convert_surface(SDL_Surface *surf, TEX &dest) {
  typedef typename TEX::pixel_type PIXELT;
  SDL_LockSurface(surf);
  if(surf == NULL) {
    throw std::runtime_error(IMG_GetError());
  }
  dest.resize(surf->w, surf->h);
  const int num_pixels = dest.get_width() * dest.get_height();
  const uint8_t *pixels = reinterpret_cast<const uint8_t*>(surf->pixels);
  if(surf->format->BytesPerPixel == 3) {
    for(int y=0; y<surf->h; ++y) {
      for(int x=0; x<surf->w; ++x) {
        ghp::color<ghp::RGB<uint8_t> > pixel;
        const uint32_t *src_pixel = reinterpret_cast<const uint32_t*>(
            pixels + y*surf->pitch + 3*x);
        uint8_t r, g, b;
        SDL_GetRGB(
          *src_pixel,
          surf->format,
          &r, &g, &b
        );
        pixel.red() = r; pixel.green() = g; pixel.blue() = b;
        dest(x, y) = pixel;
      }
    }
  } else if(surf->format->BytesPerPixel == 4) {
    for(int pix_id = 0; pix_id < num_pixels; ++pix_id) {
      ghp::color<ghp::RGBA<uint8_t> > pixel;
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
}

/**
  \brief load a texture from disk using SDL_image
  \tparam PIXELT - desired target pixel type
  \param path - path of texture to load
  \param dest - where to place the texture
 */
template<typename PIXELT>
void load_image(const std::string &path, ghp::texture<PIXELT> &dest) {
  SDL_Surface *surf = IMG_Load(path.c_str());
  if(surf == NULL) {
    throw std::runtime_error("couldn't open image for loading");
  }
  convert_surface(surf, dest);
  SDL_FreeSurface(surf);
}

/**
  \brief save a texture to disk as a BMP
  \tparam TEX - texture type
  \param path - path to save to
  \param src - texture to save
 */
template<typename TEX>
void save_bmp(const std::string &path, const TEX &src) {
  typedef typename TEX::pixel_type PIXELT;
  SDL_Surface *surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 
      src.get_width(), src.get_height(), 
      32,
      0x00FF0000,
      0x0000FF00,
      0x000000FF,
      0x00000000);
  if(surf == NULL) throw std::runtime_error(SDL_GetError());
  SDL_LockSurface(surf);
  const int num_pixels = src.get_width() * src.get_height();
  const int bpp = surf->format->BytesPerPixel;
  uint8_t *dest_pixels = reinterpret_cast<uint8_t*>(surf->pixels);
  for(int i=0; i<num_pixels; ++i) {
    uint32_t *this_pixel = reinterpret_cast<uint32_t*>(
        dest_pixels + bpp*i);
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
  \brief loads the ASCII characters from a TTF font file into a font object
  \tparam PIXELT - pixel type
  \param path - path of TTF font file
  \param pt_size - point size (based on 72DPI) to load font
  \param color - color to draw text in
  \param font - output font object
 */
template<typename PIXELT>
void load_ttf_font_ascii(const std::string &path, int pt_size, 
    const ghp::color<PIXELT> &color,
    ghp::bitmap_font<PIXELT, char> &font) {
  if(!TTF_WasInit()) {
    if(TTF_Init() == -1) {
      throw std::runtime_error(TTF_GetError());
    }
  }
  TTF_Font *ttf_font = TTF_OpenFont(path.c_str(), pt_size);
  if(ttf_font == NULL) {
    throw std::runtime_error(TTF_GetError());
  }

  // determine font characteristics
  font.set_height(TTF_FontHeight(ttf_font));
  font.set_ascent(TTF_FontAscent(ttf_font));
  font.set_descent(TTF_FontDescent(ttf_font));
  font.set_line_skip(TTF_FontLineSkip(ttf_font));

  ghp::color<ghp::RGB<uint8_t> > color_temp = color;
  SDL_Color sdl_color = { 
      color_temp.red(), 
      color_temp.green(), 
      color_temp.blue() };
  for(char c=' '; c <= '~'; ++c) {
    // set glyph graphics
    SDL_Surface *surf = TTF_RenderGlyph_Blended(ttf_font, c, sdl_color);
    if(surf == NULL) {
      TTF_CloseFont(ttf_font);
      throw std::runtime_error(TTF_GetError());
    }
    convert_surface(surf, font[c]);
    SDL_FreeSurface(surf);

    // set glyph characteristics
    int minx, maxx, miny, maxy, advance;
    if(TTF_GlyphMetrics(ttf_font, c, &minx, &maxx, 
        &miny, &maxy, &advance) != 0) {
      TTF_CloseFont(ttf_font);
      throw std::runtime_error(TTF_GetError());
    }
    font[c].set_min( ghp::vector2<int>(minx, miny) );
    font[c].set_max( ghp::vector2<int>(maxx, maxy) );
    font[c].set_advance( advance );
  }
  TTF_CloseFont(ttf_font);
}

/**
  \brief software display window from SDL.  The soft-display supports
  the texture concept.
  \tparam PIXELT - underlying pixel type
 */
template<typename PIXELT>
class soft_display_ {
public:
  typedef PIXELT pixel_type;

  /**
    \brief create a new software display window.  The window
    appears once the constructor completes, and disappears
    when this object is destroyed.
    \param width - window width
    \param height - window height
    \param fullscreen - make the window fullscreen?
   */
  soft_display_(int32_t width, int32_t height, bool fullscreen)
      : width_(width),
      height_(height),
      fullscreen_(fullscreen),
      screen_(NULL),
      texture_(width, height) {
    init_();
  }
  ~soft_display_() {
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
        tmp_color.red(), tmp_color.green(), tmp_color.blue());
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
typedef soft_display_<ghp::RGB<uint32_t> > soft_display;

/**
  \brief an opengl-enabled screen from SDL
 */
class gl_display {
public:
  gl_display(int width, int height, bool fs)
      : width_(width),
      height_(height),
      fullscreen_(fs),
      target_ticks_per_frame_(1000/30),
      last_ticks_(-1) {
    init_();
    enable_vsync(true);
  }
  ~gl_display() {
    SDL_Quit();
  }

  /** \brief set the window caption */
  inline void set_caption(const std::string &s) {
    SDL_WM_SetCaption(s.c_str(), NULL);
  }
  /** \brief update the window.  returns the number of seconds that
    passed since the last time this method was called. */
  inline float update() {
    // swap buffers, then sleep
    SDL_GL_SwapBuffers();

    const uint32_t ticks_now = SDL_GetTicks();
    const uint32_t ticks_diff = ticks_now - last_ticks_;
    if(last_ticks_ != -1) {
      if(ticks_diff != 0) {
        float this_fps = 1000. / static_cast<float>(ticks_diff);
        measured_fps_ = .99f*measured_fps_ + .01f*this_fps; // TODO: smooth with IIR filter
        const int32_t sleep_ticks = target_ticks_per_frame_ - ticks_diff;
        if(sleep_ticks > 0) {
          SDL_Delay(sleep_ticks);
        }
      } else {
        /* put up a warning here?  my laptop won't get here,
          but I relize it's becoming increasingly likely... */
      }
    }
    const uint32_t true_ticks_diff = SDL_GetTicks() - last_ticks_;
    last_ticks_ = ticks_now;
    return static_cast<float>(true_ticks_diff) / 1000.f;
  }

  /** \brief enable or disable vsync */
  inline void enable_vsync(bool enable) {
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, enable ? 1 : 0);
  }

  /** \brief gets the screen width */
  inline int get_width() const {
    return width_;
  }
  /** \brief gets the screen height */
  inline int get_height() const {
    return height_;
  }

  inline float measured_fps() const {
    return measured_fps_;
  }
  inline void set_target_fps(float f) {
    target_ticks_per_frame_ = 1000/f;
  }

  inline float aspect_ratio() const {
    return static_cast<float>(width_)/static_cast<float>(height_);
  }

  inline void grab_mouse(bool b) {
    SDL_WM_GrabInput(b ? SDL_GRAB_ON : SDL_GRAB_OFF);
    SDL_ShowCursor(b ? SDL_DISABLE : SDL_ENABLE);
  }
private:
  inline void init_() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
      throw std::runtime_error(SDL_GetError());
    }
    SDL_Surface *screen_ = SDL_SetVideoMode(width_, height_, 0, 
        SDL_OPENGL | SDL_DOUBLEBUF | (fullscreen_ ? SDL_FULLSCREEN : 0));
    if(screen_ == NULL) {
      throw std::runtime_error(SDL_GetError());
    }
  }

  int width_;
  int height_;
  bool fullscreen_;

  float measured_fps_;
  float target_ticks_per_frame_;
  uint32_t last_ticks_;
};

}

#endif

