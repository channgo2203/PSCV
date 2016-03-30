#ifndef TOP_H
#define TOP_H

#include <systemc.h>
#include <tlm.h>

#include <iostream>
 
SC_MODULE(Top) {
	SC_HAS_PROCESS(Top);

public:
   // Events
   sc_event e1;
   sc_event e2;
   
   // Constructor
   Top(sc_module_name name);

   // Destructor
   ~Top() {};

   // Definition of processes
   void thread_1();
   void thread_2();
};

#endif

