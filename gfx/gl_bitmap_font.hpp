#ifndef _GHP_GL_GL_BITMAP_FONT_HPP_
#define _GHP_GL_GL_BITMAP_FONT_HPP_

#include "gl.hpp"

#include "../gfx.hpp"
#include "../math.hpp"

#include <list>

namespace gl {

template<typename PIXELT, typename K=char>
class gl_bitmap_font : boost::noncopyable {
public:
  template<typename PIXELT2>
  gl_bitmap_font(const ghp::bitmap_font<PIXELT2, K> &font) 
      : texture_() {
    init_from_font_(font);
  }
  ~gl_bitmap_font() {
  }

  struct glyph {
  public:
    ~glyph();

  private:
    glyph();

    ghp::vector<2, float> min_;
    ghp::vector<2, float> max_;
    float advance;

  friend class gl_bitmap_font;
  };

private:
  struct font_line {
    std::list<K> chars;
    int length;

    font_line() 
        : chars(), length(0) {
    }
  };

  template<typename PIXELT2>
  void init_from_font_(const ghp::bitmap_font<PIXELT2, K> &font) {
    std::list<K> keys;
    font.get_keys(keys);

    // that old character-packing algorithm
    std::list<font_line> font_lines;
    font_lines.push_back(font_line());
    int height = font.get_height();
    for(std::list<K>::iterator it=keys.begin(); it != keys.end(); ++it) {
      // find the shortest place to put this character
      std::list<font_line>::iterator shortest = font_lines.begin();
      for(std::list<font_line>::iterator short_iter = font_lines.begin();
          short_iter != font_lines.end(); ++short_iter) {
        if( (*short_iter).length < (*shortest).length ) {
          shortest = short_iter;
        }
      }
      // choose between adding a new row and adding onto the shortest
      // row
      if( ((*shortest).length + font[*it].get_width()) < 
            (height + font.get_height()) ) {
        // add to row
        (*shortest).push_back(*it);
        (*shortest).length += font[*it].get_width();
      } else {
        // add new row
        font_lines.push_back(font_line());
        font_lines.back().push_back(*it);
        font_lines.length += font[*it].get_width();
      }
    }

    // find the longest row
    int width = 0;
    for(std::list<font_line>::iterator it = font_lines.begin();
        it != font_lines.end(); ++it) {
      if( (*it).width > width ) {
        width = (*it).width;
      }
    }

    // generate texture
    ghp::texture<PIXELT2> soft_texture(width, height);

    // push texture to GL
    texture_.write(soft_texture);

    // save glyph information
    height_ = font.get_height();
    line_skip_ = font.get_line_skip();
  }

  texture<2, PIXELT> texture_;
  std::map<K, glyph> glyphs_;
  float height_;
  float line_skip_;
};

}

#endif

