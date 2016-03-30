/* Embedded Control System modeling in SystemC
 */

/* Sensors implement the M-of-N systems (N-Modular Redundancy) 
 * meaning that the system fails when fewer than M modules 
 * are funtional
 */


#ifndef CLK_TICK_H
#define CLK_TICK_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"

SC_MODULE(Clk_tick) {
	SC_HAS_PROCESS(Clk_tick);

public:
   // tick notification
   sc_event e_tick;
   
	//constructor
	Clk_tick(sc_module_name name);
	
	// transition
	void tick();

private:
	// the spending time
	int ticks;
};

#endif
