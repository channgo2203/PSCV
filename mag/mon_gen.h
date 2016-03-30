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

#ifndef MON_GEN_H
#define MON_GEN_H

#include "global_params.h"
#include "monitor_params.h"
#include "utils.h"
#include "types.h"


class mon_gen {
 
 public:
 	// Constructor
 	mon_gen(global_params* g, 
 		    monitor_params* m,
 		    std::ostream& o,
 		    std::ostream& h);

 	// Virtual destructor
 	virtual ~mon_gen() {};

 	void monitor_header();
 	void monitor_constructor();
 	void register_with_observer();
 	void observer_callbacks();
 	void start_declaring_monitor_class_variables();
 	void finish_declaring_monitor_class_variables();
 	void step_header();
 	void step_footer();
 	void generate_value_callbacks();

 	void generate_code_for_start_of_class_definition();
 	void generate_code_for_start_of_monitor_constructor();
 	void generate_code_for_end_of_monitor_constructor();

 	// Tracing helpers
 	void trace_attributes(ssmap_t* attributetypes, ssmap_t* attributes, bool comma);
 	void trace_eventclocks(sset_t* event_clock_labels, bool comma);
 	void trace_locations(std::vector<func_loc_t*>* locs, bool comma);
 	void trace_plocations(std::vector<plocation_t*>* plocs, bool comma);
 	void trace_callbackvalues(std::vector<user_val_t*>* uv, bool comma);

 protected:
 	global_params* gp;
 	monitor_params* mp;
 	std::ostream& os;
 	std::ostream& header_os;
 	std::string this_monitor_name; // the name of the monitor class
 	std::string this_trace_filename; // the name of the trace file for this monitor
 	std::vector<std::string> kernel_clocks;

};

#endif
