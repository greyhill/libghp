#ifndef _GHP_MATH_MESH_HPP_
#define _GHP_MATH_MESH_HPP_

#include "vector.hpp"

#include <iostream>
#include <map>
#include <stdexcept>

#include <cassert>

namespace ghp {

/**
  \brief a position and normal vector (on a mesh)
  \tparam T - underlying floating point type
 */
template<typename T>
class vertex {
public:
  /** \brief create a new vertex */
  vertex() {
  }
  /** \brief create a new vertex 
    \param loc - location vector
    \param normal - normal vector */
  vertex(const vector<3, T> &loc, const vector<3, T> &normal)
      : loc_(loc),
      normal_(normal) {
  }
  ~vertex() {
  }

  /** \brief component access */
  inline vector<3, T>& location() { return loc_; }
  /** \brief component access */
  inline const vector<3, T>& location() const { return loc_; }
  /** \brief component access */
  inline vector<3, T>& normal() { return normal_; }
  /** \brief component access */
  inline const vector<3, T>& normal() const { return normal_; }

private:
  vector<3, T> loc_;
  vector<3, T> normal_;
};

/**
  \brief triangular face on a mesh
  \tparam T - underlying floating point type
  \tparam P - parent type, supporting mesh concept
 */
template<typename T, typename P>
class face {
public:
  /** \brief create a new face */
  face(P &p) : p_(p) { 
    vertices_[0] = vertices_[1] = vertices_[2] = -1;
  }
  ~face() { }

  /** \brief element access */
  inline int& ids(int i) { return vertices_[i]; }
  /** \brief element access */
  inline const int& ids(int i) const { return vertices_[i]; }
  /** \brief element access */
  inline void set_ids(int a, int b, int c) {
    vertices_[0] = a;
    vertices_[1] = b;
    vertices_[2] = c;
  }
  /** \brief element access */
  inline vertex<T>& vertices(int i) { return p_.vertices(i); }
  /** \brief element access */
  inline const vertex<T>& vertices(int i) const { return p_.vertices(i); }

private:
  P &p_;
  int vertices_[3];
};

/**
  \brief a collection of 3d vertices and faces
  \tparam T - underlying floating point type
 */
template<typename T>
class mesh {
public:
  typedef face<T, mesh<T> > face_t;

  mesh() {
  }
  ~mesh() {
  }

  /** \brief get the underlying vertex storage object */
  inline std::map<int, vertex<T> >& vertices() {
    return vertices_;
  }
  /** \brief get the underlying vertex storage object */
  inline const std::map<int, vertex<T> >& vertices() const {
    return vertices_;
  }
  /** \brief get the underlying face storage object */
  inline std::map<int, face_t>& faces() {
    return faces_;
  }
  /** \brief get the underlying face storage object */
  inline const std::map<int, face_t>& faces() const {
    return faces_;
  }

  /** \brief mutable vertex access.  if the vertex does not 
    exist, it is automatically added and returned */
  vertex<T>& vertices(int i) {
    typename std::map<int, vertex<T> >::iterator it = vertices_.find(i);
    if(it == vertices_.end()) {
      vertices_[i] = vertex<T>();
      return vertices_[i];
    } else {
      return it->second;
    }
  }
  /** \brief immutable vertex access.  attempting to access a 
    nonexistant vertex throws a std::runtime_error */
  const vertex<T>& vertices(int i) const {
    typename std::map<int, vertex<T> >::const_iterator it = vertices_.find(i);
    if(it == vertices_.end()) {
      throw std::runtime_error("vertex does not exist");
    } else {
      return it->second;
    }
  }
  /** \brief mutable face access.  if the face does not exist,
    it is automatically created and returned */
  face_t& faces(int i) {
    typename std::map<int, face_t>::iterator it = faces_.find(i);
    if(it == faces_.end()) {
      faces_[i] = face_t(*this);
      return faces_[i];
    } else {
      return it->second;
    }
  }
  /** \brief immutable face access.  if the face does not exist,
    throws a std::runtime_error */
  const face_t& faces(int i) const {
    typename std::map<int, face_t>::const_iterator it = faces_.find(i);
    if(it == faces_.end()) {
      throw std::runtime_error("face does not exist");
    } else {
      return it->second;
    }
  }

  void compact() {
    // TODO implement this
  }

private:
  std::map<int, vertex<T> > vertices_;
  std::map<int, face_t> faces_;
};

}

#endif

