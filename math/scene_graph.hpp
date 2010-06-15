#ifndef _GHP_MATH_SCENE_GRAPH_HPP_
#define _GHP_MATH_SCENE_GRAPH_HPP_

#include "spatial.hpp"
#include "../util.hpp"

#include <set>

namespace ghp {

/** \brief hierarchical scene graph
 \tparam N - dimension of scene (2 or 3)
 \tparam T - underlying floating point type
 \tparam C - type of contents
 */
template<int N, typename T, typename C>
class scene_graph {
public:
  typedef typename spatial_traits<N, T>::vector_t vector_t;
  typedef typename spatial_traits<N, T>::stable_rot_t stable_rot_t;
  typedef typename spatial_traits<N, T>::matrix_rot_t matrix_rot_t;
  typedef typename spatial_traits<N, T>::matrix_trans_t matrix_trans_t;
  typedef C value_type;

  /** \brief create a new scene_graph object
    \param loc - location relative to parent
    \param rot - rotation relative to parent
   */
  scene_graph(const vector_t &loc = vector_t(), 
      const stable_rot_t &rot = stable_rot_t())
      : loc_(loc), rot_(rot) {
  }
  /** \brief copy constructor */
  scene_graph(const scene_graph &g) {
    // perform deep copy 
    (*this) = g;
  }
  ~scene_graph() {
    for(typename std::set<scene_graph*>::const_iterator 
        it=children_.begin(); it != children_.end(); ++it) {
      erase_child(**it);
    }
  }

  /** \brief "flatten" the local coordinate spaces of the scene graph's
    hierarchy onto a single, global coordinate space
      \tparam SG - must have insert(vec_t, stable_rot_t, C)
      \param out - output scenegraph
    */
  template<typename SG>
  inline void flatten(SG &out) const {
    // identity rotation and translation
    vector_t loc;
    stable_rot_t rot;
    matrix_rot_t rot_matrix;
    flatten_helper_(out, loc, rot, rot_matrix);
  }

  /** \brief element access */
  inline const vector_t& location() const { return loc_; }
  /** \brief element access */
  inline vector_t& location() { return loc_; }
  /** \brief element access */
  inline const stable_rot_t& rotation() const { return rot_; }
  /** \brief element access */
  inline stable_rot_t& rotation() { return rot_; }
  
  /** \brief element access */
  inline const std::set<C>& contents() const { return contents_; }
  /** \brief element access */
  inline void insert(const C &c) { contents_.insert(c); }
  /** \brief element access */
  inline void erase(const C &c) { contents_.erase(c); }
  /** \brief element access */
  inline const std::set<scene_graph*>& children() const { return children_; }
  /** \brief create a new child node and return a reference to it.
         the child's memory is owned by this scene graph.
    \param loc - relative displacement
    \param rot - relative rotation
   */
  inline scene_graph& insert_child(const vector_t &loc=vector_t(), 
      const stable_rot_t &rot=stable_rot_t()) {
    scene_graph *new_child = new scene_graph(loc, rot);
    children_.insert(new_child);
    return new_child;
  }
  /** \brief remove and destroy a child */
  inline void erase_child(scene_graph &g) {
    children_.erase(&g);
    delete &g;
  }

  /** \brief deep copy */
  template<typename T2>
  scene_graph& operator=(const scene_graph<N, T2, C> &g) {
    loc_ = g.loc_;
    rot_ = g.rot_;
    contents_ = g.contents_;
    children_.erase(children_.begin(), children_.end());
    for(typename std::set< scene_graph<N, T2, C> >::const_iterator it 
        = g.contents_.begin(); it != g.contents_.end(); ++it) {
      scene_graph *g = new scene_graph();
      *g = *it;
      children_.insert(g);
    }
    return *this;
  }

private:
  template<typename SG>
  void flatten_helper_(SG &out, 
      const vector_t &rloc, 
      const stable_rot_t &rrot,
      const matrix_rot_t &rrot_matrix) const {
    vector_t cur_loc(rloc);
    cur_loc += (rrot * loc_);
    matrix_rot_t cur_rot_matrix(rrot_matrix);
    cur_rot_matrix *= matrix_rot_t(rot_);
    stable_rot_t cur_rot(rrot);
    cur_rot *= rot_;
    // add content
    for(typename std::set<C*>::const_iterator 
        it = contents_.begin(); it != contents.end(); ++it) {
      out.insert(*it, cur_loc, cur_rot);
    }
    // add children
    for(typename std::set<scene_graph*>::const_iterator 
        it = children_.begin(); it != children_.end(); ++it) {
      (*it)->flatten_helper_(out, cur_loc, cur_rot, cur_rot_matrix);
    }
  }

  vector_t loc_;
  stable_rot_t rot_;
  std::set<C> contents_;
  std::set<scene_graph*> children_;
};

}

#endif

