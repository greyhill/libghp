#include <boost/progress.hpp>

#include <ghp/util.hpp>

#include <cstdio>

#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  {
    boost::progress_timer t;
    int j;
    for(int i=0; i<100000000; ++i) {
      j = htonl(j);
    }
  }
  {
    boost::progress_timer t;
    int j;
    for(int i=0; i<100000000; ++i) {
      j = ghp::hton(j);
    }
  }

  return 0;
}

