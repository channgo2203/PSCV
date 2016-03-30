/**
 * Defines the functionality of the producer
 **/

#ifndef PRODUCER_H
#define PRODUCER_H

#include <systemc.h>
#include <tlm.h>
#include "fifo.cc"
#include "utils.h"
 
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

SC_MODULE(Producer) {
	SC_HAS_PROCESS(Producer);

public:
   // Definitions of ports
   sc_port<fifo_write_if> out; // output port

   // Constructor
   Producer(sc_module_name name, int c_init, gsl_rng *rnd);

   // Destructor
   ~Producer() {};

   // Definition of processes
   void main();

   // Writing function
   void send(char c);

private:
	int c_int;
	gsl_rng *r;
};

#endif

