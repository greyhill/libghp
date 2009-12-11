#ifndef _GHP_GL_EBO_HPP_
#define _GHP_GL_EBO_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <ghp_math.hpp>
#include <ghp_gl_bo.hpp>

namespace math = ghp::math;

namespace ghp {
namespace gl {

enum ebo_contents : GLenum {
  POINTS = GL_POINTS,
  LINES = GL_LINES,
  LINE_STRIP = GL_LINE_STRIP,
  LINE_LOOP = GL_LINE_LOOP,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
  QUADS = GL_QUADS,
  QUAD_STRIP = GL_QUAD_STRIP,
  POLYGON = GL_POLYGON
};

template<int N, typename T>
class ebo : public bo<N, T, GL_ELEMENT_ARRAY_BUFFER_ARB> {
public:
  ebo(ebo_contents contents = TRIANGLES,
    bo_type type = STATIC, bo_usage usage = DRAW)
      : bo<N, T, GL_ELEMENT_ARRAY_BUFFER_ARB>(type, usage),
      contents_(contents) {
  }

  ~ebo() {
  }

  ebo_contents get_contents() const {
    return contents_;
  }

  ebo& set_contents(ebo_contents c) {
    contents_ = c;
    return *this;
  }

  void bind() {
    bo<N, T, GL_ELEMENT_ARRAY_BUFFER_ARB>::sync_gl_();
    GLuint glTex = bo<N, T, GL_ELEMENT_ARRAY_BUFFER_ARB>
        ::get_gl_buffer_();
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glTex);
  }

  void draw(int32_t count=-1, int32_t offset=0) {
    if(count == -1) count = bo<N, T, GL_ELEMENT_ARRAY_BUFFER_ARB>
        ::get_buffer().size() * N;
    glDrawElements(contents_, count, _gl_type_<T>(), 
      reinterpret_cast<GLvoid*>(offset) );
  }

private:
  ebo_contents contents_;
};

}} // end of namespaces

#endif

