#include "producer.h"

Producer::Producer(sc_module_name name, int c_init, gsl_rng *rnd) {
	c_int = c_init;
	r = rnd; // random generator

	SC_THREAD(main);
}

void Producer::send(char c) {
	out->fifo_write(c);
	c_int = c;
}

void Producer::main() {
   	const char* str = "&abcdefgh@";
   	const char* p = str;

	while (true) {
		int b = get_bernoulli(r,0.9);
		if (b) {
			//out->fifo_write(*p);
			//c_int = *p;
			//p++;

			//if(!*p) {
			//	p = str;
			//}
			send(*p);
			p++;
			if (!*p) {
				p = str;
			}
		}
	 	
		wait(1,SC_NS); // waits for 1 nanosecond
	}	
}
