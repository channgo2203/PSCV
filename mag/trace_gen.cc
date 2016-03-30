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

#include <iostream>
#include "trace_gen.h"

// Constructor
trace_gen::trace_gen(global_params* g, 
 		monitor_params* m,
 		std::ostream& o,
 		std::ostream& h) {
	mon_robot = new mon_gen(g, m, o, h);
}

// Destructor
trace_gen::~trace_gen() {
	if (mon_robot)
		delete mon_robot;
}

void trace_gen::begin() {
	
	mon_robot->monitor_header();
    mon_robot->monitor_constructor();
    mon_robot->step_header();
}

void trace_gen::process_state() {
	mon_robot->step_footer();
}
 	
void trace_gen::end() {
	
    mon_robot->observer_callbacks();
    mon_robot->generate_value_callbacks();
    mon_robot->start_declaring_monitor_class_variables();
    mon_robot->finish_declaring_monitor_class_variables();
}

// Called by main.cc:generate_monitors()
void trace_generation(global_params* global,
	monitor_params* monitor,
	std::ostream& output,
	std::ostream& header_output) {

	// Call the constructor
	trace_gen trace_robot(global, monitor, output, header_output);

	// Generate the monitors
	trace_robot.begin();
	trace_robot.process_state();
	trace_robot.end();
}
