#ifndef _GHP_MATH_MESH_HPP_
#define _GHP_MATH_MESH_HPP_

#include <vector>

namespace ghp {

/**
  \brief a coherent collection of vertices, arranged into faces
  \tparam V - has vertex concept
 */
template<typename V>
class mesh {
public:
  /**
    \brief a triangular face
   */
  class face {
  public:
    /** \brief create a new face */
    inline face() {
      indices_[0] = indices_[1] = indices_[2] = -1;
    }
    inline ~face() {
    }

    /** \brief element access */
    inline int& operator()(int i) { return indices_[i]; }
    /** \brief element access */
    inline const int& operator()(int i) const { return indices_[i]; }
    /** \brief element access */
    inline int& operator[](int i) { return (*this)(i); }
    /** \brief element access */
    inline const int& operator[](int i) const { return (*this)(i); }

  private:
    int indices_[3];
  };

  typedef V vertex_t;
  typedef face face_t;

  /** \brief create a new mesh */
  mesh() { }
  ~mesh() { }

  /** \brief element access */
  inline vertex_t& vertices(int i) { return vertices_[i]; }
  /** \brief element access */
  inline const vertex_t& vertices(int i) const { return vertices_[i]; }
  /** \brief element access */
  inline face_t& faces(int i) { return faces_[i]; }
  /** \brief element access */
  inline const face_t& faces(int i) const { return faces_[i]; }

  /** \brief returns the number of vertices */
  inline int num_vertices() const { return vertices_.size(); }
  /** \brief returns the number of faces */
  inline int num_faces() const { return faces_.size(); }
  /** \brief allocates sufficient space for an arbitrary number of vertices */
  inline void resize_vertices(int i) { vertices_.resize(i); }
  /** \brief allocates sufficient space for an arbitrary number of faces */
  inline void resize_faces(int i) { faces_.resize(i); }

  /** \brief consolidate redundant vertices */
  void compact() { }

private:
  std::vector<vertex_t> vertices_;
  std::vector<face_t> faces_;
};

}

#endif

