/**
 * Defines the functionality of the consumer
 **/

#ifndef CONSUMER_H
#define CONSUMER_H

#include <systemc.h>
#include <tlm.h>
#include "fifo.cc"
#include "utils.h"
 
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

SC_MODULE(Consumer) {
	SC_HAS_PROCESS(Consumer);

public:
   // Definitions of ports
   sc_port<fifo_read_if> in; // input port

   // Constructor
   Consumer(sc_module_name name, int c_init, gsl_rng *rnd);

   // Destructor
   ~Consumer() {};

   // Definition of processes
   void main();

   // Reading function
   void receive(char &c);

private:
	// Reading character in ASCII
	int c_int;
	gsl_rng *r;
};

#endif

