#ifndef _GHP_GFX_FONT_HPP_
#define _GHP_GFX_FONT_HPP_

#include "color.hpp"
#include "texture.hpp"

#include "../math.hpp"

#include <map>
#include <stdexcept>

namespace ghp {

template<typename PIXELT>
class glyph {
public:
  typedef PIXELT pixel_t;

  glyph() {
  }
  ~glyph() {
  }

  inline int get_width() const { return texture_.get_width(); }
  inline int get_height() const { return texture_.get_height(); }
  inline int get_rows() const { return texture_.get_rows(); }
  inline int get_cols() const { return texture_.get_cols(); }
  inline void resize(int width, int height) { 
    texture_.resize(width, height); 
  }

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

  int get_advance() const { return advance_; }
  void set_advance(int i) { advance_ = i; }

private:
  texture<pixel_t> texture_;
  int advance_;
};

template<typename PIXELT, typename K=char>
class font {
public:
  typedef PIXELT pixel_t;
  typedef glyph<pixel_t> glyph_t;

  font() {
  }
  font(const font &f)
      : glyphs_(f.glyphs_) {
  }
  ~font() {
  }

  inline glyph_t& operator()(const K &k) {
    typename std::map<K, boost::shared_ptr<glyph_t> >::iterator 
        it = glyphs_.find(k);
    if(it == glyphs_.end()) {
      boost::shared_ptr<glyph_t> new_glyph(new glyph_t());
      glyphs_[k] = new_glyph;
      return *new_glyph;
    } else {
      return *(it->second);
    }
  }
  const glyph_t& operator()(const K &k) const {
    typename std::map<K, boost::shared_ptr<glyph_t> >::const_iterator 
        it = glyphs_.find(k);
    if(it == glyphs_.end()) {
      throw std::runtime_error("font doesn't contain that glyph!");
    } else {
      return *(it->second);
    }
  }
  inline glyph_t& operator[](const K &k) {
    return (*this)(k);
  }
  inline const glyph_t& operator[](const K &k) const {
    return (*this)(k);
  }

private:
  std::map<K, boost::shared_ptr<glyph_t> > glyphs_;
};

}

#endif

