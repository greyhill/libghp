#ifndef _GHP_MATH_MESH_HPP_
#define _GHP_MATH_MESH_HPP_

#include "vector.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

#include <cassert>

namespace ghp {

template<typename T>
class vertex {
public:
  vertex() {
  }
  vertex(const vector<3, T> &loc, const vector<3, T> &normal)
      : loc_(loc),
      normal_(normal) {
  }
  ~vertex() {
  }

  inline vector<3, T>& location() { return loc_; }
  inline const vector<3, T>& location() const { return loc_; }
  inline vector<3, T>& normal() { return normal_; }
  inline const vector<3, T>& normal() const { return normal_; }

private:
  vector<3, T> loc_;
  vector<3, T> normal_;
};

}

#endif

