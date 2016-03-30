/* Embedded control system modeling in SystemC
 */


/* Actuators implement the M-of-N systems (N-Modular Redundancy) 
 * meaning that the system fails when fewer than M modules 
 * are funtional
 */


#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"

SC_MODULE(Actuator) {
	SC_HAS_PROCESS(Actuator);

public:
	// constructor
	Actuator(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Actuator();
	
	// transition
	void actuator_transition();
	
	// get current state
	int get_currentstate();
	
	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	int a;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in the current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;
};

#endif

