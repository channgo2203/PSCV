/* Embedded Control System modeling in SystemC
 */


#include "clk_tick.h"



Clk_tick::Clk_tick(sc_module_name name) {
	// initialization
	ticks = 0;

	SC_THREAD(tick);
}


void Clk_tick::tick() {
	while (true) {
		// notify the end of tick
		e_tick.notify(SC_ZERO_TIME);
		
		// tick it
		wait(time_unit, SC_MS);	
		ticks = ticks + 1;
	
	}
}
