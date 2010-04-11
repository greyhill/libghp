#ifndef _GHP_GFX_TEXTURE_UTIL_HPP_
#define _GHP_GFX_TEXTURE_UTIL_HPP_

#include "texture.hpp"

#include "../math.hpp"

#include <cassert>

namespace ghp {

/**
  \brief directly copy source pixels to destination
    pixels
 */
struct direct_copy {
  /**
    \brief perform the copying operation
    \tparam T1 - must support texture concept
    \tparam T2 - must support texture concept
   */
  template<typename T1, typename T2>
  void operator()(const T1 &src, 
      int sx, int sy, T2 &out) {
    typedef typename T1::pixel_t PIXELT;
    if( (sx >= 0) && (sx < src.get_width())
        && (sy >= 0) && (sy < src.get_height()) ) {
      out = src(sx, sy);
    } else {
      out = color<PIXELT>();
    }
  }
};

/**
  \brief copy several source pixels and perform an
    unweighted average
  \tparam SIZE - 2*SIZE+1 will be the dimension of the
    square of pixels averaged 
 */
template<int SIZE>
struct average_copy {
  /**
    \brief perform the averaging operation
    \tparam T1 - must support texture concept
    \tparam T2 - must support texture concept
   */
  template<typename T1, typename T2>
  void operator()(const T1 &src,
      int sx, int sy, T2 &out) {
    typedef typename T1::pixel_t PIXELT1;
    typedef typename T2::pixel_t PIXELT2;
    color<PIXELT1> pixel;
    float num_pixels = 0;
    for(int y = std::max(0, sy-SIZE); 
       y <= std::min(sy+SIZE, src.get_height()-1); ++y) {
     for(int x = std::max(0, sx-SIZE);
         x <= std::min(sx+SIZE, src.get_width()-1); ++x) {
       pixel += src(x,y);
       ++num_pixels;
      }
    }
    pixel /= num_pixels;
    out = pixel;
  }
};

/**
  \brief scale a texture from one size to another
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
  \tparam T1 - source type, must support texture concept
  \tparam T2 - destination type, must support texture concept
 */
template<typename SRCGET, typename T1, typename T2>
inline void scale_texture(
    const T1 &src,
    T2 &dest) {
  typedef typename T1::pixel_t PIXELT1;
  typedef typename T2::pixel_t PIXELT2;
  SRCGET srcget;
  for(int dy = 0; dy < dest.get_height(); ++dy) {
    const int sy = linear_interpolate(
      0, src.get_height(),
      0, dest.get_height(),
      dy);
    for(int dx = 0; dx < dest.get_width(); ++dx) {
      const int sx = linear_interpolate(
        0, src.get_width(),
        0, dest.get_width(),
        dx);
      color<PIXELT1> pixel;
      srcget(src, sx, sy, pixel);
      dest(dx, dy) = pixel;
    }
  }
}

/**
  \brief rotate a texture
  \tparam PIXELT - source pixel type
  \tparam PIXELT2 - destination pixel type
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
 */
template<typename PIXELT, typename PIXELT2, typename SRCGET>
void rotate_texture_long(
    const texture<PIXELT> &src,
    float degrees,
    texture<PIXELT2> &dest) {
}

/**
  \brief rotate a texture
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
  \tparam T1 - source texture type
  \tparam T2 - destination texture type
 */
template<typename SRCGET, typename T1, typename T2>
inline void rotate_texture(const T1 &src, float degrees, T2 &dest) {
  typedef typename T1::pixel_t PIXELT;
  typedef typename T2::pixel_t PIXELT2;
  SRCGET srcget;
  const float rad = -degrees * M_PI/180.0;
  for(int dy=0; dy < dest.get_height(); ++dy) {
    const float ndy = dy - dest.get_height()/2;
    for(int dx=0; dx < dest.get_width(); ++dx) {
      const float ndx = dx - dest.get_width()/2;
      const float c = cosf(rad);
      const float s = sinf(rad);
      const int nsx = (ndx*c + ndy*s);
      const int nsy = (-ndx*s + ndy*c);
      const int sx = nsx + src.get_width()/2;
      const int sy = nsy + src.get_height()/2;
      color<PIXELT> pixel;
      srcget(src, sx, sy, pixel);
      dest(dx, dy) = pixel;
    }
  }
}

/**
  \brief convert an image between pixel types
  \tparam PIXELT - source pixel type
  \tparam PIXELT2 - destination pixel type
 */
template<typename T1, typename T2>
void convert_texture(const T1 &src, T2 &dst) {
  dst.resize(src.get_rows(), src.get_cols());
  for(int dr=0; dr<dst.get_rows(); ++dr) {
    for(int dc=0; dc<dst.get_cols(); ++dc) {
      dst(dr, dc) = src(dr, dc); // should trigger the cast operator
    }
  }
}

/**
  \brief draw a line on a texture
  \tparam TEX - texture type with texture concept
  \tparam C - color type with color concept
  \param texture - destination texture
  \param color - color of line to be drawn
  \param p1 - starting (x,y) coordinates
  \param p2 - ending (x,y) coordinates
 */
template<typename TEX, typename C>
void draw_line(TEX &texture, const C &color,
    const vector<2, int> &p1, const vector<2, int> &p2) {
  // we ``walk'' the cursor down from one coordinate to 
  // the other
  int x = p1(0);
  int y = p1(1);
  const int xdiff = std::abs(p2(0) - x);
  const int ydiff = std::abs(p2(1) - y);
  const int xsign = signum(p2(0) - x);
  const int ysign = signum(p2(1) - y);
  if(xdiff > ydiff) { 
    const float slope = ydiff / xdiff;
    float accum = 0;
    for(int i=0; i<xdiff; ++i, accum += slope) {
      texture(x, y) = color;
      x += xsign;
      if(accum > 1) {
        y += ysign;
        accum -= 1;
      }
    }
  } else {
    const float slope = xdiff / ydiff;
    float accum = 0;
    for(int i=0; i<ydiff; ++i, accum += slope) {
      texture(x, y) = color;
      y += ysign;
      if(accum > 1) {
        x += xsign;
        accum -= 1;
      }
    }
  }
}

template<typename TEX1, typename TEX2>
void blit_texture(const TEX1 &src, TEX2 &dst, 
    const vector<2, int> &src_p, const vector<2, int> &src_size,
    const vector<2, int> &dst_p) {
  for(int y=0; y<src_size(1); ++y) {
    for(int x=0; x<src_size(0); ++x) {
      vector<2, int> dst_v(dst_p);
      dst_v += vector2<int>(x, y);
      vector<2, int> src_v(src_p);
      src_v += vector2<int>(x, y);
      dst(dst_v) = src(src_v);
    }
  }
}

template<typename TEX1, typename TEX2>
inline void blit_texture(const TEX1 &src, TEX2 &dst,
    const vector<2, int> &dst_p) {
  blit_texture(src, dst, vector2<int>(0, 0),
    vector2<int>(src.get_width(), src.get_height()),
    dst_p);    
}

template<typename TEX, typename D, typename C, typename V>
void plot_texture(TEX &texture, const D &data, int size,
    const C &color, const V &min_y, const V &max_y) {
  assert(max_y > min_y);
  vector<2, int> last_pt = vector2<int>(
    0,
    linear_interpolate(texture.get_height()-1, 0,
      min_y, max_y, data[0])
  );
  for(int i=1; i<texture.get_width(); ++i) {
    int idx = linear_interpolate(0, size, 0, texture.get_width()-1, i);
    vector<2, int> this_pt = vector2<int>(
      i,
      linear_interpolate(texture.get_height()-1, 0,
        min_y, max_y, data[idx])
    );
    draw_line(texture, color, last_pt, this_pt);
    last_pt = this_pt;
  }
}

template<typename TEX, typename D, typename C, typename V>
inline void plot_texture(TEX &texture, const D &data,
    const C &color, const V &min_y, const V &max_y) {
  plot_texture(texture, data, data.size(), color, min_y, max_y);
}

template<typename TEX, typename C>
void fill_texture(TEX &texture, const C &color) {
  for(int y=0; y<texture.get_height(); ++y) {
    for(int x=0; x<texture.get_width(); ++x) {
      texture(x, y) = color;
    }
  }
}

}

#endif

