#ifndef _GHP_GFX_FONT_HPP_
#define _GHP_GFX_FONT_HPP_

#include "color.hpp"
#include "texture.hpp"

#include "../math.hpp"

#include <map>

namespace ghp {

template<typename PIXELT>
class glyph {
public
  typedef PIXELT pixel_t;

  glyph() {
  }
  ~glyph() {
  }

  inline int get_width() const { return texture_.get_width(); }
  inline int get_height() const { return texture_.get_height(); }
  inline int get_rows() const { return texture_.get_rows(); }
  inline int get_cols() const { return texture_.get_cols(); }
  inline void resize(int width, int height) { texture_.resize(width, height); }

  inline color<pixel_t>& operator()(int x, int y) {
    return texture_(x, y);
  }
  inline const color<pixel_t>& operator()(int x, int y) const {
    return texture_(x, y);
  }
  inline color<pixel_t>& operator()(const vector<2, int> &v) {
    return texture_(v);
  }
  inline const color<pixel_t>& operator()(const vector<2, int> &v) const {
    return texture_(v);
  }
  inline color<pixel_t>& operator[](int i) {
    return texture_[i];
  }
  inline const color<pixel_t>& operator[](int i) const {
    return texture_[i];
  }

private:
  texture<pixel_t> texture_;
  vector<2, int> glyph_dims_;
  int advance_;
};

template<typename PIXELT, typename K=char>
class font {
public:
  typedef PIXELT pixel_t;
  typedef glyph<pixel_t> glyph_t;

  font();
  font(const font &f);
  ~font();

  glyph& operator()(const K &k);
  const glyph& operator()(const K &k) const;
  glyph& operator[](const K &k);
  const glyph& operator()(const K &k) const;

private:
  std::map<K, boost::shared_ptr<glyph_t> glyphs_;
};

}

#endif

