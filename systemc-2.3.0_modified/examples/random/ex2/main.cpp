/*
 * Created by: Chan Ngo, INRIA, 2015
 * P1:start executes before P1:end and P2:end and
 * P2:start executes before P1:end and P2:end
 * However, the execution order of P1:start and P2:start is arbitrary
 * In the same way, the execution order of P1:end and P2:end is arbitrary
 * In total, there are 4 execution orders
 */
 
#include <cstdlib>
#include <time.h>
#include <iostream>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "top.h"

int sc_main(int argc, char *argv[]) {
  // top module
  Top top_module("top_module");
  sc_set_time_resolution(1,SC_NS); // time resolution
  
  // seed the generator
  //unsigned long int seed = time(NULL) * getpid();
  //std::cout << "Provided seed: " << seed << std::endl;
  sc_set_random_seed();
  //sc_set_random_seed(seed);
  
  sc_start();

  return 0;
}
