/* Embedded control system modeling in SystemC
 */


/* Input processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCI_H
#define PROCI_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"


SC_MODULE(Proci) {
	SC_HAS_PROCESS(Proci);

public:
	// ports
	sc_out < int > input_reboot;

	// constructor
	Proci(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Proci();

	// transition
	void proci_transition();
	
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
	static int proci_status;
	
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
