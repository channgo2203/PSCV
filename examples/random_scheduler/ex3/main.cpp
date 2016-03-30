/*
 * P1:start executes before P1:end and P2:end and
 * thread_1: segments S1, S4, S7
 * thread_2: segments S2, S5, S8
 * thread_3: segments S3, S6, S9
 * An execution has form of {S1,S2,S3}-{S4,S5,S6}-{S7,S8,S9}
 * In total, there are (3!).(3!).(3!) = 216 execution orders
 */
 
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "top.h"

unsigned int execution_register[EXECUTION_LENGTH];

int sc_main(int argc, char *argv[]) {
  // write execution to file
  std::ofstream exfile("ex.txt",std::ofstream::binary | std::ofstream::trunc);
  if (! exfile.good()) {
  	std::cerr << "Unable to open \"ex.txt\" for output." << std::endl;
    exit (1);
  }
  
  // top module
  Top top_module("top_module");
  sc_set_time_resolution(1,SC_NS); // time resolution
  
  // seed the generator
  unsigned long int seed = time(NULL) * getpid();
  std::cout << "Provided seed: " << seed << std::endl;
  //sc_set_random_seed();
  sc_set_random_seed(seed);
  
  sc_start();
  
  std::cout << "Execution order: ";
  for (int i = 0; i < EXECUTION_LENGTH; i++)
  	std::cout << execution_register[i] << " ";
  std::cout << std::endl;
		
  for (int i = 0; i < EXECUTION_LENGTH; i++) {
  	exfile << execution_register[i];
  	if (i < EXECUTION_LENGTH - 1)
  		exfile << " ";
  	else 
  		exfile << std::endl;
  }
  exfile.flush();
  exfile.close();
  
  return 0;
}
