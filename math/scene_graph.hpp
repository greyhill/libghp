#ifndef _GHP_MATH_SCENE_GRAPH_HPP_
#define _GHP_MATH_SCENE_GRAPH_HPP_

#include "spatial.hpp"
#include "../util.hpp"

#include <set>

namespace ghp {

template<int N, typename T, typename C>
class scene_graph {
public:
  typedef typename spatial_traits<N, T>::vector_t vector_t;
  typedef typename spatial_traits<N, T>::stable_rot_t stable_rot_t;
  typedef typename spatial_traits<N, T>::matrix_rot_t matrix_rot_t;
  typedef typename spatial_traits<N, T>::matrix_trans_t matrix_trans_t;

  template<SG>
  inline void flatten(SG &out) const {
    // identity rotation and translation
    vector_t loc;
    stable_rot_t rot;
    matrix_rot_t rot_matrix;
    flatten_helper_(out, loc, rot, rot_matrix);
  }
  
  inline const std::set<C*>& contents() const { return contents_; }
  inline void add_content(C *c) { contents_.insert(c); }
  inline void remove_content(C *c) { contents_.erase(c); }
  inline const std::set<scene_graph*>& children() const { return children_; }

private:
  template<SG>
  void flatten_helper_(SG &out, 
      const vector_t &rloc, 
      const stable_rot_t &rrot,
      const matrix_rot_t &rrot_matrix) const {
    vector_t cur_loc(rloc);
    cur_loc += (rrot * loc_);
    matrix_rot_t cur_rot_matrix(rrot_matrix);
    cur_rot_marix *= matrix_rot_t(rot_);
    stable_rot_t cur_rot(rrot);
    cur_rot *= rot_;
    // add content
    for(std::set<C*>::const_iterator it=contents_.begin(); 
        it != contents.end(); ++it) {
      out.add_content(*it, cur_loc, cur_rot);
    }
    // add children
    for(std::set<scene_graph*>::const_iterator it = children_.begin();
        it != children_.end(); ++it) {
      (*it)->flatten_helper_(out, cur_loc, cur_rot, cur_rot_matrix);
    }
  }

  vector_t loc_;
  stable_rot_t rot_;
  std::set<C*> contents_;
  std::set<scene_graph*> children_;
};

}

#endif

