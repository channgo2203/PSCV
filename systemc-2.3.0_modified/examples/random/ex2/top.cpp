#include "top.h"

Top::Top(sc_module_name name) {
	
	SC_THREAD(thread_1);
	SC_THREAD(thread_2);
}

void Top::thread_1() {
	std::cout << "P1: start." << std::endl;
	e1.notify(SC_ZERO_TIME); //delta-cycle notification delayed
	
	wait(e2);
	std::cout << "P1: end." << std::endl;
}

void Top::thread_2() {
	std::cout << "P2: start." << std::endl;
	e2.notify(SC_ZERO_TIME); //delta-cycle notification delayed
	
	wait(e1);
	std::cout << "P2: end." << std::endl;
}