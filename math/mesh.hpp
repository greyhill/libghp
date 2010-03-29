#ifndef _GHP_MATH_MESH_HPP_
#define _GHP_MATH_MESH_HPP_

#include "vector.hpp"

#include <cassert>

namespace ghp {

/** \brief class for accessing indices of a triangle in a mesh
 */
class ref_triangle {
public:
  ref_triangle() {
  }
  ref_triangle(int a, int b, int c) {
    indices_[0] = a;
    indices_[1] = b;
    indices_[2] = c;
  }
  ~ref_triangle() {
  }

  /** \brief element access */
  inline int& operator()(int i) {
    return indices_[i];
  }
  /** \brief element access */
  inline const int& operator()(int i) const {
    return indices_[i];
  }
  /** \brief element access */
  inline int& operator[](int i) {
    return (*this)(i);
  }
  /** \brief element access */
  inline const int& operator[](int i) const {
    return (*this)(i);
  }

private:
  int indices_[3];
};

/**
  \brief abstract representation of 3d mesh composed of triangles
  \tparam T - floating-point type used, e.g. float or double
 */
template<typename T>
class mesh {
public:
  typedef vector<3, T> vertex_t;
  typedef vector<3, T> normal_t;

  /** \brief create an empty mesh */
  mesh() {
  }
  /** \brief create an empty mesh
    \param num_vertices - allocate space for this many normals and locations
    \param num_triangles - allocate space for this many faces */
  mesh(int num_vertices, int num_triangles)
      : vertices_(num_vertices),
      normals_(num_vertices),
      triangles_(num_triangles) {
  }
  /** \brief copy ctor */
  mesh(const mesh &m) 
      : vertices_(m.vertices_),
      normals_(m.normals_),
      triangles_(m.triangles_) {
  }
  ~mesh() {
  }

  /** \brief element access */
  inline std::vector<vector<3, T> >& vertices() {
    return vertices_;
  }
  /** \brief element access */
  inline const std::vector<vector<3, T> >& vertices() const {
    return vertices_;
  }
  /** \brief element access */
  inline std::vector<vector<3, T> >& normals() {
    return normals_;
  }
  /** \brief element access */
  inline const std::vector<vector<3, T> >& normals() const {
    return normals_;
  }
  /** \brief element access */
  inline std::vector<ref_triangle<T> >& triangles() {
    return triangles_;
  }
  /** \brief element access */
  inline const std::vector<ref_triangle<T> >& triangles() const {
    return triangles_;
  }

  mesh& operator=(const mesh &m) {
    vertices_ = m.vertices_;
    normals_ = m.normals_;
    triangles_ = m.triangles_;
  }

private:
  std::vector<vector<3, T> > vertices_;
  std::vector<vector<3, T> > normals_;
  std::vector<ref_triangle<T> > triangles_;
};

}

#endif

