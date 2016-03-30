#include "top.h"

extern unsigned int execution_register[EXECUTION_LENGTH];

Top::Top(sc_module_name name) {
	
	SC_THREAD(thread_1);
	SC_THREAD(thread_2);
	SC_THREAD(thread_3);
	
	for (int i = 0; i < EXECUTION_LENGTH; i++) {
		execution_register[i] = 0; //0 : no segment was registered
	}
	index = 0;
}

void Top::thread_1() {
	execution_register[index] = 1;
	index++;
	e1.notify(SC_ZERO_TIME);
	
	wait(e2 & e3);
	execution_register[index] = 4;
	index++;
	e1.notify(SC_ZERO_TIME);
	
	wait(e2 & e3);
	execution_register[index] = 7;
	index++;
}

void Top::thread_2() {
	execution_register[index] = 2;
	index++;
	e2.notify(SC_ZERO_TIME);
	
	wait(e1 & e3);
	execution_register[index] = 5;
	index++;
	e2.notify(SC_ZERO_TIME);
	
	wait(e1 & e3);
	execution_register[index] = 8;
	index++;
}

void Top::thread_3() {
	execution_register[index] = 3;
	index++;
	e3.notify(SC_ZERO_TIME);
	
	wait(e1 & e2);
	execution_register[index] = 6;
	index++;
	e3.notify(SC_ZERO_TIME);
	
	wait(e1 & e2);
	execution_register[index] = 9;
	index++;
}