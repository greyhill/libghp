#ifndef _GHP_GFX_ARRAY_BUFFER_HPP_
#define _GHP_GFX_ARRAY_BUFFER_HPP_

#include "generic_buffer_object.hpp"
#include "../math.hpp"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdint.h>

namespace ghp {

template<int N, typename T>
class vertex_buffer : public
    generic_buffer_object<vector<N, T>, GL_ARRAY_BUFFER_ARB> {
private:
  typedef generic_buffer_object<vector<N, T>, GL_ARRAY_BUFFER_ARB> gbo_type;
public:
  vertex_buffer()
      : generic_buffer_object<vector<N, T>, GL_ARRAY_BUFFER_ARB>() { }
  template<typename PTR>
  vertex_buffer(std::size_t size, PTR ptr)
      : generic_buffer_object<vector<N, T>, GL_ARRAY_BUFFER_ARB>(size, ptr) 
      { }
  vertex_buffer(const vertex_buffer &b)
      : generic_buffer_object<vector<N, T>, GL_ARRAY_BUFFER_ARB>(b) { }
  ~vertex_buffer() { }

private:
  friend class renderer;
};

}

#endif

