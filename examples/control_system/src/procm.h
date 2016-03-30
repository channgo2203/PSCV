/* Embedded control system modeling in SystemC
 */


/* Main processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCM_H
#define PROCM_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"

SC_MODULE(Procm) {
	SC_HAS_PROCESS(Procm);

public:
	// ports
	sc_out < int > timeout;

	// constructor
	Procm(sc_module_name name, int initstate, int initcount, gsl_rng *rnd);
	
	// destructor
	~Procm();

	// transition
	void procm_transition();

	// count transition
	void procm_timeout_transition();
	
	// get current state
	static int get_currentstate();

	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	static int procm_status;
	
	// timeout cycle count
	int count;

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

	// timeout holding time
	double timeout_holdingtime;
	
	// the random generator
	gsl_rng *r;
};

#endif
