#ifndef TOP_H
#define TOP_H

#define EXECUTION_LENGTH	9

#include <systemc.h>
#include <tlm.h>

#include <iostream>

SC_MODULE(Top) {
	SC_HAS_PROCESS(Top);

public:
	// Execution index
	unsigned int index;
	
   	// Events
   	sc_event e1;
   	sc_event e2;
   	sc_event e3;
   
   	// Constructor
   	Top(sc_module_name name);

   	// Destructor
   	~Top() {};

	// Definition of processes
   	void thread_1();
   	void thread_2();
   	void thread_3();
};

#endif

