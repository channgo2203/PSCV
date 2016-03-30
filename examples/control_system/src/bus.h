/* Embedded Control System modeling in SystemC
 */

/* Bus implementation,
 *the communication is modelled using buffers 
 */


#ifndef BUS_H
#define BUS_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"

SC_MODULE(Bus) {
	SC_HAS_PROCESS(Bus);

public:
	// ports to connect to processors
	sc_in < int > input_reboot;
	sc_in < int > output_reboot;
	sc_in < int > timeout;

	// constructor
	Bus(sc_module_name name, int initcomp, int initreqi, int initreqo, gsl_rng *rnd);
	
	~Bus();

	// transition
	void receiving_input_reboot();
	void receiving_output_reboot();
	void receiving_timeout();

	static int get_comp();

private:
	// module name
	std::string mname;

	// main processor has processed data from input processor
	// and sent corresponding instructions to output 
	// processor (since last timeout), init 1
	static int comp;
	
	// input processor has data ready to send, init 1
	int reqi;
	
	// output processor has instructions ready to be processed, init 0
	int reqo;
	
	// the random generator
	gsl_rng *r;
};

#endif
