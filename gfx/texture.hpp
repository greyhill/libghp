#ifndef _GHP_GFX_TEXTURE_HPP_
#define _GHP_GFX_TEXTURE_HPP_

#include "color.hpp"
#include <ghp/math.hpp>

#include <boost/scoped_array.hpp>

#include <algorithm>

#include <cassert>
#include <cmath>
#include <stdint.h>

namespace ghp {

/** \brief texture class, representing a 2D field of colors
  \tparam PIXELT - some pixel type, like RGB<uint8_t>
 */
template<typename PIXELT>
class texture {
public:
  typedef PIXELT pixel_t;

  /** \brief create a new 0x0 texture
   */
  texture()
      : width_(0),
      height_(0),
      data_(NULL) {
  }
  /** \brief create a new texture of the given dimensions
   */
  texture(int32_t width, int32_t height) 
      : width_(width),
      height_(height),
      data_(new color<PIXELT>[width*height]) {
  }
  /** \brief copy constructor */
  texture(const texture &t) 
      : width_(t.width_),
      height_(t.height_),
      data_(new color<PIXELT>[width_*height_]) {
    std::copy(
      &t.data_[0],
      &t.data_[0] + (width_ * height_),
      &data_[0]
    );
  }
  ~texture() {
  }

  /** \brief copy a texture */
  texture& operator=(const texture &t) {
    resize(t.width_, t.height_);
    std::copy(
      &t.data_[0],
      &t.data_[0] + (width_ * height_),
      &data_[0]
    );
  }

  /** \brief returns the height of the texture */
  inline int32_t get_rows() const {
    return height_;
  }
  /** \brief returns the width of the texture */
  inline int32_t get_cols() const {
    return width_;
  }
  /** \brief returns the width of the texture */
  inline int32_t get_width() const {
    return width_;
  }
  /** \brief returns the height of the texture */
  inline int32_t get_height() const {
    return height_;
  }
  /** \brief resizes the texture's bounds.  THIS DOES
    NOT MANIPULATE THE PIXELS IN ANY WAY.  check out 
    the texture manipulation functions for 
    "photoshop-like" resizing */
  void resize(int32_t width, int32_t height) {
    bool resize = (width * height) != (width_ * height_);
    width_ = width;
    height_ = height;
    if(resize) {
      data_.reset(new color<PIXELT>[width_*height_]);
    }
  }

  /** \brief pixel access */
  inline color<PIXELT>& operator()(int32_t x, int32_t y) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width_);
    assert(y < height_);
    return data_[width_ * y + x];
  }
  /** \brief pixel access */
  inline const color<PIXELT>& operator()(int32_t x, int32_t y) const {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width_);
    assert(y < height_);
    return data_[width_ * y + x];
  }
  /** \brief pixel access */
  inline color<PIXELT>& operator()(const vector<2, int> &v) {
    return (*this)(v(0), v(1));
  }
  /** \brief pixel access */
  inline const color<PIXELT>& operator()(const vector<2, int> &v) const {
    return (*this)(v(0), v(1));
  }
  /** \brief pixel access */
  inline color<PIXELT>& operator()(int32_t i) {
    assert(i >= 0);
    assert(i < width_ * height_);
    return data_[i];
  }
  /** \brief pixel access */
  inline const color<PIXELT>& operator()(int32_t i) const {
    assert(i >= 0);
    assert(i < width_ * height_);
    return data_[i];
  }
  /** \brief pixel access */
  inline color<PIXELT>& operator[](int32_t i) {
    return (*this)(i);
  }
  /** \brief pixel access */
  inline const color<PIXELT>& operator[](int32_t i) const {
    return (*this)(i);
  }

private:
  int32_t width_;
  int32_t height_;
  boost::scoped_array<color<PIXELT> > data_;
};

struct direct_copy {
  template<typename PIXELT>
  void operator()(const texture<PIXELT> &src, 
      int sx, int sy, color<PIXELT> &out) {
    if( (sx >= 0) && (sx < src.get_width())
        && (sy >= 0) && (sy < src.get_height()) ) {
      out = src(sx, sy);
    } else {
      out = color<PIXELT>();
    }
  }
};

template<int SIZE>
struct average_copy {
  template<typename PIXELT>
  void operator()(const texture<PIXELT> &src,
      int sx, int sy, color<PIXELT> &out) {
    color<PIXELT> pixel;
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
  \tparam PIXELT - source pixel type
  \tparam PIXELT2 - destination pixel type
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
 */
template<typename PIXELT, typename PIXELT2, typename SRCGET>
void scale_texture_long(
    const texture<PIXELT> &src,
    texture<PIXELT2> &dest) {
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
      color<PIXELT> pixel;
      srcget(src, sx, sy, pixel);
      dest(dx, dy) = pixel;
    }
  }
}

/**
  \brief scale a texture from one size to another
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
  \tparam T1 - source texture type
  \tparam T2 - destination texture type
 */
template<typename SRCGET, typename T1, typename T2>
inline void scale_texture(
    const T1 &src,
    T2 &dest) {
  scale_texture_long<typename T1::pixel_t, 
    typename T2::pixel_t, SRCGET>(src, dest);
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
  \brief rotate a texture
  \tparam SRCGET - pixel extractor, like direct_copy or average_copy
  \tparam T1 - source texture type
  \tparam T2 - destination texture type
 */
template<typename SRCGET, typename T1, typename T2>
inline void rotate_texture(const T1 &src, float degrees, T2 &dest) {
  rotate_texture_long<typename T1::pixel_t, typename T2::pixel_t, SRCGET>
    (src, degrees, dest);
}

/**
  \brief convert an image between pixel types
  \tparam PIXELT - source pixel type
  \tparam PIXELT2 - destination pixel type
 */
template<typename PIXELT, typename PIXELT2>
void convert_texture(const texture<PIXELT> &src,
    texture<PIXELT2> &dst) {
  dst.resize(src.get_rows(), src.get_cols());
  for(int dr=0; dr<dst.get_rows(); ++dr) {
    for(int dc=0; dc<dst.get_cols(); ++dc) {
      dst(dr, dc) = src(dr, dc); // should trigger the cast operator
    }
  }
}

}

#endif

