#ifndef _GHP_GL_VBO_HPP_
#define _GHP_GL_VBO_HPP_

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

enum vbo_contents : uint8_t {
  VERTICES = 0,
  NORMALS = 1,
  TEXCOORDS = 2
};

template<int N, typename T>
class vbo : public bo<N, T, GL_ARRAY_BUFFER_ARB> {
public:
  vbo(vbo_contents contents = VERTICES,
    bo_type type = STATIC, bo_usage usage = DRAW)
      : bo<N, T, GL_ARRAY_BUFFER_ARB>(type, usage),
      contents_(contents) {
  }

  ~vbo() {
  }

  vbo_contents get_contents() const {
    return contents_;
  }

  vbo& set_contents(vbo_contents c) {
    contents_ = c;
    return *this;
  }

  void bind() {
    // ugh, C++ can be one ugly son of a gun
    bo<N, T, GL_ARRAY_BUFFER_ARB>::sync_gl_();
    GLuint glTex = bo<N, T, GL_ARRAY_BUFFER_ARB>::get_gl_buffer_();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, glTex);
    switch(contents_) {
      case VERTICES:
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(N, _gl_type_<T>(), 0, 0);
        break;
      case NORMALS:
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(_gl_type_<T>(), 0, 0);
        break;
      case TEXCOORDS:
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(N, _gl_type_<T>(), 0, 0);
        break;
      default:
        std::cerr << "Invalid vbo contents_ field"
          << std::endl;
        assert(0);
    }
  }
  
private:
  vbo_contents contents_;
};

}} // end of namespaces

#endif

