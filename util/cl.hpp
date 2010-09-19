#ifndef _GHP_UTIL_CL_HPP_
#define _GHP_UTIL_CL_HPP_

#include <cl.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cl {



template<int UNUSED>
class context_ : boost::noncopyable {
public:
  context()
  ~context();

private:
};
typedef context_<0> context;

}

#endif

