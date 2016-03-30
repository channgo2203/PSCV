#include <cstdlib>
#include <time.h>
#include <iostream>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include <systemc.h>
#include <tlm.h>
#include "sysc/kernel/sc_simcontext.h"

SC_MODULE(top) {
public:
  void thread_1();
  void thread_2();

  SC_CTOR(top) {
    SC_THREAD(thread_1);
    SC_THREAD(thread_2);
  }
};

void top::thread_1() {
  std::cout << "thread_1 is executing." << std::endl;
}

void top::thread_2() {
  std::cout << "thread_2 is executing." << std::endl;
}

int sc_main(int argc, char *argv[]) {
  // top module
  top top_module("top_module");
  
  sc_set_time_resolution(1,SC_NS);
  
  // seed the generator
  unsigned long int seed = time(NULL) * getpid();
  std::cout << "provided seed: " << seed << std::endl;
  sc_set_random_seed(seed);
  
  sc_start();

  sc_set_random_seed(1); // no effect, only shows a warning

  return 0;
}
