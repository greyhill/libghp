#ifndef _GHP_JANUS_JANUS_ASSEMBLY_HPP_
#define _GHP_JANUS_JANUS_ASSEMBLY_HPP_

#include <stdint.h>

namespace ghp {

template<typename T>
class checked_buffer {
public:
  janus_assembly();
  janus_assembly(const std::string &path);
  ~janus_assembly();

private:
};

}

#endif

