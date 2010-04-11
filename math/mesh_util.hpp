#ifndef _GHP_MATH_MESH_UTIL_HPP_
#define _GHP_MATH_MESH_UTIL_HPP_

#include "vector.hpp"
#include "vertex_aux.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cstdio>

namespace ghp {

/** 
  \brief load a wavefront OBJ mesh
  \tparam M - supports mesh concept
  \param path - path of file to load
  \param m - mesh in which to store result 
 */
template<typename M>
void load_obj_mesh(const std::string &path, M &m) {
  typedef typename M::vertex_t vertex_t;

  // collection datatypes
  std::vector<vector<3, float> > locs;
  std::vector<vector<3, float> > norms;
  std::vector<vector<2, float> > uvs;
  std::vector<boost::array<boost::array<int, 3>, 3> > faces;

  std::stringstream ss;
  std::ifstream in(path.c_str());
  while(!in.eof()) {
    // get and clean a line
    std::string line;
    std::getline(in, line);
    boost::to_lower(line);
    boost::trim(line);
    ss.clear();
    ss.str(line);

    // determine the line type and read data
    std::string line_type;
    ss >> line_type;
    if(line_type == "v") { // location
      vector<3, float> v;
      ss >> v(0) >> v(1) >> v(2);
      locs.push_back(v);
    } else if(line_type == "vn") { // normal
      vector<3, float> v;
      ss >> v(0) >> v(1) >> v(2);
      norms.push_back(v);
    } else if(line_type == "vt") { // uv coordinates
      vector<2, float> v;
      ss >> v(0) >> v(1);
      uvs.push_back(v);
    } else if(line_type == "f") { // face
      boost::array<boost::array<int, 3>, 3> face;
      for(int v=0; v<3; ++v) {
        std::string v_str;
        boost::array<int, 3> this_v;
        ss >> v_str;
        // LOC/UV/NORM
        if(sscanf(v_str.c_str(), "%d/%d/%d", 
            &this_v[0], &this_v[1], &this_v[2]) == 3) {
          face[v] = this_v;
        } else if(sscanf(v_str.c_str(), "%d/%d//",
            &this_v[0], &this_v[1]) == 2) {
          face[v] = this_v;
        } else if(sscanf(v_str.c_str(), "%d//%d",
            &this_v[0], &this_v[2]) == 2) {
          face[v] = this_v;
        } else if(sscanf(v_str.c_str(), "%d/%d",
            &this_v[0], &this_v[2]) == 2) {
          face[v] = this_v;
        } else {
          throw std::runtime_error(
            "couldn't understand OBJ file (must be triangularized!)");
        }
      }
      faces.push_back(face);
    }
  }

  // inefficiently form the mesh
  m.resize_faces(faces.size());
  m.resize_vertices(3 * faces.size());
  for(int face=0; face<faces.size(); ++face) {
    for(int v=0; v<3; ++v) {
      vertex_write_loc<vertex_t>()(m.vertices(3*face+v),
        locs[faces[face][v][0] - 1]);
      vertex_write_norm<vertex_t>()(m.vertices(3*face+v),
       norms[faces[face][v][2] - 1]);
      vertex_write_uv<vertex_t>()(m.vertices(3*face+v),
        uvs[faces[face][v][1] - 1]);
    }
  }

  // clean up aforementioned inefficiencies
  m.compact();
}

}

#endif

