/* Embedded control system modeling in SystemC
 */


/* Input processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCO_H
#define PROCO_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"

SC_MODULE(Proco) {
	SC_HAS_PROCESS(Proco);

public:
	// ports
	sc_out < int > output_reboot;

	// constructor
	Proco(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Proco();

	// transition
	void proco_transition();
	
	// get current state
	static int get_currentstate();
	
	// get number of reboots
	static int get_number_reboots();

	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	static int proco_status;
	
	// number of reboots
	static int number_reboot;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;
};

#endif
