#ifndef _GHP_GL_BITMAP_FONT_HPP_
#define _GHP_GL_BITMAP_FONT_HPP_

#include "gl.hpp"

#include "../gfx.hpp"
#include "../math.hpp"

#include <list>
#include <utility>

namespace gl {

template<typename PIXELT, typename K=char>
class bitmap_font : boost::noncopyable {
public:
  template<typename PIXELT2>
  bitmap_font(const ghp::bitmap_font<PIXELT2, K> &font) 
      : texture_() {
    init_from_font_(font);
  }
  ~bitmap_font() {
  }

  /** \brief gl bitmap containing all the characters in this font */
  texture<2, PIXELT>& texture();
  /** \brief gl vbo containing texture coordaintes for this font */
  vbo<GL_ARRAY_BUFFER>& texture_coords();

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
    for(typename std::list<K>::iterator it=keys.begin(); 
        it != keys.end(); ++it) {
      // find the shortest place to put this character
      typename std::list<font_line>::iterator shortest = font_lines.begin();
      for(typename std::list<font_line>::iterator short_iter 
          = font_lines.begin();
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
        (*shortest).chars.push_back(*it);
        (*shortest).length += font[*it].get_width();
      } else {
        // add new row
        font_lines.push_back(font_line());
        font_lines.back().chars.push_back(*it);
        font_lines.back().length += font[*it].get_width();
        height += font.get_height();
      }
    }

    // find the longest row
    int length = 0;
    for(typename std::list<font_line>::iterator it = font_lines.begin();
        it != font_lines.end(); ++it) {
      if( (*it).length > length ) {
        length = (*it).length;
      }
    }

    // generate texture, coordinate vbo
    height += font.get_height();
    ghp::texture<PIXELT2> soft_texture(length, height);
    ghp::vector<2, int> soft_texture_cursor = ghp::vector2<int>(0,0);
    std::vector<ghp::vector<2, float> > glyph_coords;
    int glyph_idx = 0;
    
    for(typename std::list<font_line>::iterator line_iter = font_lines.begin();
        line_iter != font_lines.end(); ++line_iter) {
      const font_line &line = *line_iter;
      soft_texture_cursor(0) = 0;
      for(typename std::list<K>::const_iterator char_iter = line.chars.begin();
          char_iter != line.chars.end(); ++char_iter) {
        const ghp::glyph<PIXELT2> &glyph = font[*char_iter];

        // draw every image down a little bit so baseline appears
        // at the same position for every glyph
        ghp::vector<2, int> draw_cursor = soft_texture_cursor + 
            ghp::vector2<int>( 0, font.get_height() - glyph.get_max()(1) );
        ghp::blit_texture(glyph, soft_texture, draw_cursor);

        // generate location of glyph in texture and save it to vbo
        ghp::vector<2, float> upper_left = ghp::vector2<float>(
            soft_texture_cursor(0) / static_cast<float>(length),
            soft_texture_cursor(1) / static_cast<float>(height) );
        ghp::vector<2, float> lower_right = ghp::vector2<float>(
            (soft_texture_cursor(0) + glyph.get_width()) 
                / static_cast<float>(length),
            soft_texture_cursor(1) / static_cast<float>(height) );
        glyph_coords.push_back(lower_right);
        glyph_coords.push_back(ghp::vector2<float>(
            lower_right(0), upper_left(1)));
        glyph_coords.push_back(upper_left);
        glyph_coords.push_back(ghp::vector2<float>(
            upper_left(0), lower_right(1)));

        // save glyph advance and ... etc.
        std::pair<int, float> idx_and_advance( glyph_idx, 
            glyph.get_advance() );
        glyph_info_[*char_iter] = idx_and_advance;

        // advance cursor
        soft_texture_cursor(0) += glyph.get_width();
        ++glyph_idx;
      }
      soft_texture_cursor(1) += font.get_height();
    }

    // push texture, vbo to GL
    texture_.write(soft_texture);
    glyph_coords_.write(glyph_coords, 
        sizeof(ghp::vector<2,float>)*glyph_coords.size());

    // save font information
    height_ = font.get_height();
    line_skip_ = font.get_line_skip();
  }

  texture<2, PIXELT> texture_;
  std::map<K, std::pair<int, float> > glyph_info_;
  vbo<GL_ARRAY_BUFFER_ARB> glyph_coords_;
  float height_;
  float line_skip_;
};

}

#endif

