/*
	Copyright (C) 2014 INRIA
	
	This file is part of MAG.
    MAG is a free software that generates monitor and aspect files for SystemC

    with monitor generated from Linear Temporal Logic assertions.
   
    CHIMP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CHIMP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CHIMP. See the file COPYING.
    If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TRACE_GEN_H
#define TRACE_GEN_H

#include <iostream>
#include "mon_gen.h"

class trace_gen {

 public:
 	// Constructor
 	trace_gen(global_params*, 
 		monitor_params*,
 		std::ostream&,
 		std::ostream&);

 	// Destructor
 	~trace_gen();

 	void begin();
 	void process_state();
 	void end();

 private:
 	mon_gen* mon_robot;
};

void trace_generation(global_params* global,
	monitor_params* monitor,
	std::ostream& output,
	std::ostream& header_output);

#endif
