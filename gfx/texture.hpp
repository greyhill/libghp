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
  typedef PIXELT pixel_type;

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

}

#endif

