#include <ghp/util/cl.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  // display available platforms and devices!
  const std::list<const cl::platform*> &platforms 
      = cl::platform::get_platforms();
  for(std::list<const cl::platform*>::const_iterator it = platforms.begin();
      it != platforms.end(); ++it) {
    std::cout << "Platform: " << (*it)->name() << 
      ", " << (*it)->version() << " from " << (*it)->vendor()
      << " [" << (*it)->profile() << "] supports extensions: " << std::endl;

    // display supported extensions
    const std::set<std::string>& extns = (*it)->extensions();
    for(std::set<std::string>::const_iterator eit = extns.begin();
        eit != extns.end(); ++eit) {
      std::cout << *eit << std::endl;
    }
    std::cout << std::endl;

    const std::list<cl::device>& devices = (*it)->devices();
  }
}

