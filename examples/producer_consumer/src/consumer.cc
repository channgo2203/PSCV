#include "consumer.h"

Consumer::Consumer(sc_module_name name, int c_init, gsl_rng *rnd) {
	c_int = c_init;
	r = rnd; // random generator

	SC_THREAD(main);
}

void Consumer::receive(char &c) {
	in->fifo_read(c);
	c_int = c;
}

void Consumer::main() {
	char c;
   	while (true) {
   		// use the Bernoulli distribution in GSL
   		int b = get_bernoulli(r,0.9);
   		if (b) {
   			//in->fifo_read(c);
   			//c_int = c;
   			receive(c);
   		}
	
		wait(1,SC_NS); // waits for 1 nanosecond
	}	
}
