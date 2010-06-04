#ifndef _GHP_GFX_FONT_HPP_
#define _GHP_GFX_FONT_HPP_

#include "color.hpp"
#include "texture.hpp"

#include "../math.hpp"

#include <map>
#include <stdexcept>

namespace ghp {

/**
  \brief an individual textured glyph in a font. supports the
  texture concept.
  \tparam PIXELT - some pixel type
 */
template<typename PIXELT>
class glyph {
public:
  typedef PIXELT pixel_type;

  /** \brief create a new glyph */
  glyph() {
  }
  /** \brief copy constructor */
  glyph(const glyph &g) 
      : texture_(g.texture_),
      advance_(g.advance_) {
  }
  ~glyph() {
  }

  /** \brief element access */
  inline int get_width() const { return texture_.get_width(); }
  /** \brief element access */
  inline int get_height() const { return texture_.get_height(); }
  /** \brief element access */
  inline int get_rows() const { return texture_.get_rows(); }
  /** \brief element access */
  inline int get_cols() const { return texture_.get_cols(); }
  /** \brief element access */
  inline void resize(int width, int height) { 
    texture_.resize(width, height); 
  }

  /** \brief pixel access */
  inline color<pixel_type>& operator()(int x, int y) {
    return texture_(x, y);
  }
  /** \brief pixel access */
  inline const color<pixel_type>& operator()(int x, int y) const {
    return texture_(x, y);
  }
  /** \brief pixel access */
  inline color<pixel_type>& operator()(const vector<2, int> &v) {
    return texture_(v);
  }
  /** \brief pixel access */
  inline const color<pixel_type>& operator()(const vector<2, int> &v) const {
    return texture_(v);
  }
  /** \brief pixel access */
  inline color<pixel_type>& operator()(int i) {
    return texture_(i);
  }
  /** \brief pixel access */
  inline const color<pixel_type>& operator()(int i) const {
    return texture_(i);
  }
  /** \brief pixel access */
  inline color<pixel_type>& operator[](int i) {
    return texture_[i];
  }
  /** \brief pixel access */
  inline const color<pixel_type>& operator[](int i) const {
    return texture_[i];
  }

  /** \brief element access.  The distance in pixels to advance
    between this glyph's origin and the next */
  inline int get_advance() const { return advance_; }
  /** \brief element access.  The distance in pixels to advance
    between this glyph's origin and the next */
  inline void set_advance(int i) { advance_ = i; }

  /** \brief element access */
  inline const vector<2, int> get_max() const { return max_; }
  /** \brief element access */
  inline void set_max(const vector<2, int> &v) { max_ = v; }

  /** \brief element access */
  inline const vector<2, int> get_min() const { return min_; }
  /** \brief element access */
  inline void set_min(const vector<2, int> &v) { min_ = v; }

private:
  texture<pixel_type> texture_;
  vector<2, int> min_;
  vector<2, int> max_;
  int advance_;
};

/**
  \brief a bitmapped font
  \tparam PIXELT - pixel type
  \tparam K - character type, defaults to char
 */
template<typename PIXELT, typename K=char>
class font {
public:
  typedef PIXELT pixel_type;
  typedef glyph<pixel_type> glyph_type;

  /** \brief create a new font */
  font() {
  }
  /** \brief copy constructor */
  font(const font &f)
      : glyphs_(f.glyphs_) {
  }
  ~font() {
  }

  /** \brief mutable glyph access.  If a glyph is requested does
    not exist, it is created, added to the font, and returned.  */
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
  /** \brief immutable glyph access.  If a glyph requested does not
    exist yet, an exception is thrown. */
  const glyph_t& operator()(const K &k) const {
    typename std::map<K, boost::shared_ptr<glyph_t> >::const_iterator 
        it = glyphs_.find(k);
    if(it == glyphs_.end()) {
      throw std::runtime_error("font doesn't contain that glyph!");
    } else {
      return *(it->second);
    }
  }
  /** \brief mutable glyph access.  If a glyph is requested does
    not exist, it is created, added to the font, and returned.  */
  inline glyph_t& operator[](const K &k) {
    return (*this)(k);
  }
  /** \brief immutable glyph access.  If a glyph requested does not
    exist yet, an exception is thrown. */
  inline const glyph_t& operator[](const K &k) const {
    return (*this)(k);
  }

  /** \brief element access */
  inline int get_height() const { return height_; }
  /** \brief element access */
  inline void set_height(int i) { height_ = i; }
  /** \brief element access */
  inline int get_ascend() const { return ascend_; }
  /** \brief element access */
  inline void set_ascend(int i) { ascend_ = i; }
  /** \brief element access */
  inline int get_descent() const { return descent_; }
  /** \brief element access */
  inline void set_descent(int i) { descent_ = i; }
  /** \brief element access */
  inline int get_line_skip() const { return line_skip_; }
  /** \brief element access */
  inline void set_line_skip(int i) { line_skip_ = i; }

private:
  std::map<K, boost::shared_ptr<glyph_t> > glyphs_;
  int height_;
  int ascend_;
  int descent_;
  int line_skip_;
};

}

#endif

