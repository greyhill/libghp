#ifndef _GHP_GFX_SDL_HPP_
#define _GHP_GFX_SDL_HPP_

#include <ghp/gfx.hpp>

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>
#include <string>

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
void write_texture(const std::string &path, const ghp::texture<PIXELT> &src) {
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

}

#endif

