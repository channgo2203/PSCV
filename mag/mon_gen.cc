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

#include <math.h>
#include "mon_gen.h"


// Constructor
mon_gen::mon_gen(global_params* g, 
 		    monitor_params* m,
 		    std::ostream& o,
 		    std::ostream& h) :
	gp(g),
	mp(m),
	os(o),
	header_os(h) {

  kernel_clocks.push_back("MON_INIT_PHASE_BEGIN");
  kernel_clocks.push_back("MON_INIT_PHASE_END");
  kernel_clocks.push_back("MON_INIT_UPDATE_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_INIT_UPDATE_PHASE_END"); 
  kernel_clocks.push_back("MON_INIT_DELTA_NOTIFY_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_INIT_DELTA_NOTIFY_PHASE_END"); 
  kernel_clocks.push_back("MON_DELTA_CYCLE_BEGIN"); 
  kernel_clocks.push_back("MON_DELTA_CYCLE_END"); 
  kernel_clocks.push_back("MON_EVALUATION_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_EVALUATION_PHASE_END"); 
  kernel_clocks.push_back("MON_UPDATE_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_UPDATE_PHASE_END"); 
  kernel_clocks.push_back("MON_DELTA_NOTIFY_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_DELTA_NOTIFY_PHASE_END"); 
  kernel_clocks.push_back("MON_TIMED_NOTIFY_PHASE_BEGIN"); 
  kernel_clocks.push_back("MON_TIMED_NOTIFY_PHASE_END"); 
  kernel_clocks.push_back("MON_METHOD_SUSPEND"); 
  kernel_clocks.push_back("MON_THREAD_SUSPEND");
}


/*
 * Generate and print paramaters of the monitor
 */
void mon_gen::monitor_header() {
	std::string id;
	mp->get_id_str(&id);
	this_monitor_name = gp->get_mon_name() + id;

  // trace file name
  this_trace_filename = this_monitor_name + ".trace";

  os << "/*" << std::endl 
	   << " * Monitor's formula:" << std::endl 
     << " */" << std::endl << std::endl;

	std::string commout;
	mp->to_string(&commout);
	comment_out_string(&commout);
	os << commout;
	os << std::endl << std::endl << std::endl;
}


/*
 * Generate the definition of class in the header file
 */
void mon_gen::generate_code_for_start_of_class_definition() {
  
  header_os << "/*" << std::endl 
            << " * Monitor class declaration" << std::endl 
            << " */" << std::endl << std::endl;

  header_os << "class " << this_monitor_name
		    << " : public sc_core::mon_prototype {"
		    << std::endl << std::endl;

  header_os << "public:" << std::endl << std::endl;
}


/*
 * Generate the constructor definition in the cc file
 */
void mon_gen::generate_code_for_start_of_monitor_constructor() {

  header_os << "  /*" << std::endl 
            << "   * Constructor" << std::endl 
            << "   */" << std::endl << std::endl;

  // The constructor's arguments
  header_os << "  " << this_monitor_name << "(sc_core::mon_observer* obs";
  
  std::stringstream constructor_arguments;
  int i = 0;
  for (ssmap_t::const_iterator it = gp->get_vartypes()->begin(); 
    it != gp->get_vartypes()->end(); 
  	++it) {
  	constructor_arguments << ", " << it->second << " obj" << i;
    i++;
  }

  header_os << constructor_arguments.str() << ");" << std::endl;

  // Destructor declaration
  header_os << std::endl;
  header_os << "  /*" << std::endl 
            << "   * Destructor" << std::endl 
            << "   */" << std::endl << std::endl;
  header_os << "  " << "~" << this_monitor_name << "();";
  header_os << std::endl;

  // Constructor definition in the cc file
  os << "/*" << std::endl 
  	 << " * Constructor" << std::endl 
  	 << " */" << std::endl << std::endl;
  
  os << this_monitor_name << "::" << this_monitor_name 
  	 << "(sc_core::mon_observer* obs"
     << constructor_arguments.str() << ") : sc_core::mon_prototype() {" 
     << std::endl << std::endl;

  os << "  // Observer" << std::endl;
  os << "  observer = obs;" << std::endl;
  	
  // Initialize the class attributes
  os << "  // Usertypes" << std::endl;
  i = 0;
  for (ssmap_t::const_iterator it = gp->get_vartypes()->begin(); 
      it != gp->get_vartypes()->end();
      ++it ) {
    os << "  " << it->first << " = obj" << i << ";" << std::endl;
    i++;
  }

  // Initialize the location primitives
  std::vector<func_loc_t*>* locs = gp->get_func_locs();
  os << "  // Locations" << std::endl;
  for (unsigned int i = 0; i < locs->size(); i++) {
    os << "  " << locs->at(i)->loc_name << " = false;" << std::endl;
  }

  // Initialize the plocation primitives
  std::vector<plocation_t*>* plocs = gp->get_plocations();
  os << "  // Plocations" << std::endl;
  for (unsigned int i = 0; i < plocs->size(); i++) {
    os << "  " << plocs->at(i)->loc_name << " = false;" << std::endl;
  }

  // Initialize the value callbacks
  std::vector<user_val_t*>* uv = gp->get_user_vals();
  os << "  // Value callbacks" << std::endl;
  for (unsigned int i = 0; i < uv->size(); i++) {
    user_val_t* this_uv = uv->at(i);
    os << "  " << this_uv->val_name << " = 0;" << std::endl;
  }

  // Initialize the module attributes - nothing to do
  os << "  // Attributes - nothing to initialization" << std::endl;

  // Initialize the event clock monitor variables
  sset_t* clocks_set = mp->get_clocks();
  sset_t* event_clock_labels = gp->get_event_clock_labels_subset(clocks_set);

  os << "  // Event clocks" << std::endl;
  for (sset_t::iterator it = event_clock_labels->begin(); 
    it != event_clock_labels->end(); 
    ++it) {

    std::string clock_label = *it;
    os << "  " << clock_label << " = false;" << std::endl;
  }

  delete event_clock_labels;
 
  if (gp->get_write_to_file()) {
    // Initialize the trace file
    os << "  // Trace file" << std::endl;
    // trace file declaration
    os << "  std::ofstream* tracefile;" << std::endl;
    os << "  tracefile = new std::ofstream("
      << "\"" << this_trace_filename << "\", "
      << " std::ofstream::out);"  
      << std::endl << std::endl;

    os << "  if (! tracefile->good()) {" << std::endl;
    os << "    std::cerr << \"Unable to open " << this_trace_filename << " for output.\"" 
      << " << std::endl;" << std::endl;
    os << "    std::cout << \"\\n# plasma-error Unable to open "
      << this_trace_filename << " for output. #\\n\" << std::endl;" << std::endl;
    os << "    exit (1);" << std::endl;
    os << "  }" << std::endl << std::endl;

    os << "  if (tracefile) {" << std::endl 
      << "    tracefile->close();" << std::endl  
      << "    delete tracefile;" << std::endl 
      << "  }" << std::endl << std::endl;
  }

  // Register with observer
  register_with_observer();
}


/*
 * Generate the end of the end constructor and destruction definitions
 */
void mon_gen::generate_code_for_end_of_monitor_constructor() {
   
  os << "}" << std::endl << std::endl;
  
  // Destructor
  os << "/*" << std::endl 
     << " * Destructor" << std::endl 
     << " */" << std::endl << std::endl;
  
  os << this_monitor_name << "::~" << this_monitor_name 
     << "() {" << std::endl 
     << std::endl;  

  os << "}" << std::endl << std::endl;

}


/*
 * Generate the declaration of the monitor class up to the construction
 */
void mon_gen::monitor_constructor() {

	generate_code_for_start_of_class_definition();

	generate_code_for_start_of_monitor_constructor();

	generate_code_for_end_of_monitor_constructor();
}


void mon_gen::register_with_observer() {

  os << "  // Register with observer" << std::endl;

  sset_t* clocks_set = mp->get_clocks();
  std::string obs_name = "observer";
  
  // loop through all clocks in the formula
  for (sset_t::iterator it = clocks_set->begin();
    it != clocks_set->end();
    ++it) {
    std::string clk = *it;

    #ifdef DEBUG_TRACE_GEN
      std::cerr << "The clock is " << clk << std::endl;
    #endif
      
    if (strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
  
      #ifdef DEBUG_TRACE_GEN
        std::cerr << "Generating callbacks for the default clock" << std::endl;
      #endif
  
      for (unsigned int i = 0; i < kernel_clocks.size(); i++) {
        os << "  " << obs_name << "->register_monitor(this, "
           << kernel_clocks[i] << ");" << std::endl;
    
        #ifdef DEBUG_TRACE_GEN
          std::cerr << "Generated a callbacks for i = " << i << ": "<< kernel_clocks[i] << std::endl;
        #endif
      }
    }
  
    else {
      // Only register the kernel clocks
      for (unsigned int i = 0; i < kernel_clocks.size(); i++) {
        if (strcasecmp(clk.c_str(), kernel_clocks[i].c_str()) == 0) {
          os << "  " << obs_name << "->register_monitor(this, " << clk << ");" << std::endl;
        }
      }
    }

    // Event
    if (gp->is_event_clock(&clk)) {
      std::string* event_clock = gp->get_event_clock(&clk);
      std::string* event_name = get_event_from_clock(event_clock);
    
      os << "  " << obs_name << "->register_monitor(this, &(" << *event_name << "));" << std::endl;
      
      delete event_clock;
      delete event_name;
    }

  }
}


void mon_gen::step_header() {

  header_os << std::endl;
  header_os << "  /*" << std::endl;
  header_os << "   * Simulate a step of the monitor." << std::endl;
  header_os << "   */" << std::endl << std::endl;
  header_os << "  void step();" << std::endl;

  os << "/*" << std::endl;
  os << " * Simulate a step of the monitor." << std::endl;
  os << " */" << std::endl << std::endl;
  os << "void" << std::endl;
  os << this_monitor_name << "::step() {" << std::endl;

  /* Do not need it any more
  // Check if the number of states reaches the trace length
  os << "  // Check if the number of states reaches the trace length" << std::endl;
  os << "  numberofstates++;" << std::endl;
  os << "  if (numberofstates > tracelength) {" << std::endl;
  os << "    sc_stop();" << std::endl;
  os << "    return;" << std::endl;
  os << "  }" << std::endl << std::endl;
  */

  // Get command from standard input - stdin
  os << "  //Get command from standard input - stdin" << std::endl;
  os << "  std::string cmd;" << std::endl << std::endl;
  os << "  while (true) {" << std::endl;
  os << "    std::getline(std::cin, cmd);" << std::endl;
  os << "    // Get quit command" << std::endl;
  os << "    if (strcmp(cmd.c_str(), \"#plasma-quit#\") == 0) {" << std::endl;
  os << "      sc_stop();" << std::endl;
  os << "      std::cout << \"\\n# plasma-quit at \" << sc_time_stamp() << \" #\\n\" << std::endl;" << std::endl;
  os << "      exit (0);" << std::endl;
  os << "    }" << std::endl << std::endl;

  os << "    // Get an invalid command" << std::endl;
  os << "    else if (strcmp(cmd.c_str(), \"#plasma-newstate#\") != 0) {" << std::endl;
  os << "      // Wait for a valid command" << std::endl;
  os << "      continue;" << std::endl;
  os << "    }" << std::endl << std::endl;

  os << "    // Get a new state request" << std::endl;
  os << "    else {" << std::endl;
  os << "      // Process the command" << std::endl;
  os << "      break;" << std::endl;
  os << "    }" << std::endl;
  os << "  }" << std::endl << std::endl;

}


/*
 * Tracing helpers
 * Datatype supported is primitive datatype supported by SystemC
 * sc_bit, sc_bv<n>, sc_logic, sc_lv<n>, sc_int<n> (up to 64), sc_uint<n> (up to 64), sc_bigint<n>, sc_biguint<n>
 * bool, int, usigned int, long, insigned long, signed char, unsigned char, short, unsigned short, enum, struct fields
 */
void mon_gen::trace_attributes(ssmap_t* attributetypes, ssmap_t* attributes, bool comma) {
   unsigned int i = 0;
  
  if ((attributes->size() > 0) && comma) {
    // write a comma
    os << "  outputstate << " << "\", \"" << ";" << std::endl;
  }

  for (ssmap_t::const_iterator it = attributes->begin();
    it != attributes->end();
    it++) {
    
    ssmap_t::iterator typeit;
    typeit = attributetypes->find(it->first);
    if (typeit != attributetypes->end()) {
        if (typeit->second == "bool") {
            
            os << "  if (" << it->second << ") {" << std::endl;
            os << "    outputstate << " << "\"" << it->first << "\"" << ";" << std::endl;
            os << "    outputstate << " << "\" : \"" << ";" << std::endl;
            os << "    outputstate << " << "\"" << "true" << "\"" << ";" << std::endl;
            
            if (++i < attributes->size()) {
                os << "    outputstate << " << "\", \"" << ";" << std::endl;
            }
            os << "  }" << std::endl;
            
            os << "  else {" << std::endl;
            os << "    outputstate << " << "\"" << it->first << "\"" << ";" << std::endl;
            os << "    outputstate << " << "\" : \"" << ";" << std::endl;
            os << "    outputstate << " << "\"" << "false" << "\"" << ";" << std::endl;
            
            if (++i < attributes->size()) {
                os << "    outputstate << " << "\", \"" << ";" << std::endl;
            }
            os << "  }" << std::endl;
        }
        else {
            // sc_bit : '0' or '1'
            // sc_bv<n> : string of 0 and 1
            // sc_logic : '0', '1', 'X', and 'Z'
            // sc_lv<n> : string of 0, 1, X, and Z
            // sc_int<n> : convert to int 
            // sc_uint<n> : convert to int 
            // sc_bigint<n> : convert to int 
            // sc_biguint<n> : convert to int   
            // C++ types automatically with stringstream except bool
            // enum values are from 0
            // all above datatypes can be write automatically with stringstream
            
            os << "  outputstate << " << "\"" << it->first << "\"" << ";" << std::endl;
            os << "  outputstate << " << "\" : \"" << ";" << std::endl;
            os << "  outputstate << " << it->second << ";" << std::endl;
            
            
            if (++i < attributes->size()) {
                os << "  outputstate << " << "\", \"" << ";" << std::endl;
            }
            
            os << std::endl;
        }
    }
    else {
        std::cerr << "Your attribute definitions are not correct!" << std::endl;
        exit (1);
    }
  }
}


void mon_gen::trace_eventclocks(sset_t* event_clock_labels, bool comma) {

  unsigned int i = 0;

  if ((event_clock_labels->size() > 0) && comma) {
     os << "  outputstate << " << "\", \"" << ";" << std::endl;
  }
  
  for (sset_t::iterator it = event_clock_labels->begin(); 
    it != event_clock_labels->end(); 
    ++it) {
      
    std::string clock_label = *it;
    os << "  if (" << clock_label << ") {" << std::endl;
    os << "    outputstate << " << "\"" << clock_label << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "true" << "\"" << ";" << std::endl;
    
    if (++i < event_clock_labels->size()) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;

    os << "  else {" << std::endl;
    os << "    outputstate << " << "\"" << clock_label << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "false" << "\"" << ";" << std::endl;
      
    if (++i < event_clock_labels->size()) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;
  }   
  
}


void mon_gen::trace_locations(std::vector<func_loc_t*>* locs, bool comma) {

  if ((locs->size() > 0) && comma) {
    os << "  outputstate << " << "\", \"" << ";" << std::endl;
  }

  for (unsigned int i = 0; i < locs->size(); i++) {

    os << "  if (" << locs->at(i)->loc_name << ") {" << std::endl;
    os << "    outputstate << " << "\"" << locs->at(i)->loc_name << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "true" << "\"" << ";" << std::endl;
    
    if (i < locs->size() - 1) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;

    os << "  else {" << std::endl;
    os << "    outputstate << " << "\"" << locs->at(i)->loc_name << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "false" << "\"" << ";" << std::endl;
    
    if (i < locs->size() - 1) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;
  }
}


void mon_gen::trace_plocations(std::vector<plocation_t*>* plocs, bool comma) {

  if ((plocs->size() > 0) && comma) {
    os << "  outputstate << " << "\", \"" << ";" << std::endl;
  }
  
  for (unsigned int i = 0; i < plocs->size(); i++) {

    os << "  if (" << plocs->at(i)->loc_name << ") {" << std::endl;
    os << "    outputstate << " << "\"" << plocs->at(i)->loc_name << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "true" << "\"" << ";" << std::endl;
    
    if (i < plocs->size() - 1) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;

    os << "  else {" << std::endl;
    os << "    outputstate << " << "\"" << plocs->at(i)->loc_name << "\"" << ";" << std::endl;
    os << "    outputstate << " << "\" : \"" << ";" << std::endl;
    os << "    outputstate << " << "\"" << "false" << "\"" << ";" << std::endl;
    
    if (i < plocs->size() - 1) {
      os << "    outputstate << " << "\", \"" << ";" << std::endl;
    }
    os << "  }" << std::endl;
  }
}


void mon_gen::trace_callbackvalues(std::vector<user_val_t*>* uv, bool comma) {
  
  if ((uv->size() > 0) && comma) {
    os << "  outputstate << " << "\", \"" << ";" << std::endl;
  }

  for (unsigned int i = 0; i < uv->size(); i++) {
    
    user_val_t* this_uv = uv->at(i);

    os << "  outputstate << " << "\"" << this_uv->val_name << "\"" << ";" << std::endl;
    os << "  outputstate << " << "\" : \"" << ";" << std::endl;
    os << "  outputstate << " << this_uv->val_name << ";" << std::endl;

    if (i < uv->size() - 1) {
      os << "  outputstate << " << "\", \"" << ";" << std::endl;
    }
  }
}


/* 
 * Tracing function
 */
void mon_gen::step_footer() {
  bool comma = false;
  // attributetypes
  ssmap_t* attributetypes = gp->get_attributetypes();
    
  // attributes
  ssmap_t* attributes = gp->get_attributelabels();
  
  // event clocks
  sset_t* clocks_set = mp->get_clocks();
  sset_t* event_clock_labels = gp->get_event_clock_labels_subset(clocks_set);
  
  // locations
  std::vector<func_loc_t*>* locs = gp->get_func_locs();
  
  // plocations
  std::vector<plocation_t*>* plocs = gp->get_plocations();
  
  // callback values
  std::vector<user_val_t*>* uv = gp->get_user_vals();

  if ((attributes->size() > 0) ||
      (event_clock_labels->size() > 0) ||
      (locs->size() > 0) ||
      (plocs->size() > 0) ||
      (uv->size() > 0)
     ) {

	// Initialization output string of current state
	os << "  std::stringstream outputstate;" << std::endl;
	os << "  outputstate.str ("");" << std::endl;
	os << "  outputstate << \"\\n# plasma-state [ (\";"
	   << std::endl << std::endl;

    if (gp->get_write_to_file()) {
	     // Initialization tracefile
       os << "  std::ofstream* tracefile = new std::ofstream(" << "\"" << this_trace_filename << "\", "
          << " std::ofstream::app);"  
          << std::endl;
       os << "  if (! tracefile->good()) {" << std::endl;
       os << "    std::cerr << \"Unable to open " << this_trace_filename << " for output.\"" 
          << " << std::endl;" << std::endl;
       os << "    std::cout << \"\\n# plasma-error Unable to open "
          << this_trace_filename << " for output. #\\n\" << std::endl;" << std::endl;
       os << "    exit (1);" << std::endl;
       os << "  }" << std::endl << std::endl;
    }

    // Log attributes values
    os << std::endl << "  // Log attributes values" << std::endl;
    trace_attributes(attributetypes, attributes, comma);
    os << std::endl;
  
    // Log event clocks
    os << std::endl << "  // Log event clocks - Boolean" << std::endl;
    comma = (attributes->size() > 0);
    trace_eventclocks(event_clock_labels, comma);
    os << std::endl;

    // Log locations
    os << std::endl << "  // Log locations - Boolean" << std::endl;
    comma = ((attributes->size() > 0) || (event_clock_labels->size() > 0));
    trace_locations(locs, comma);
    os << std::endl;

    // Log pattern locations - plocation
    os << std::endl << "  // Log pattern locations - plocation - Boolean" << std::endl;
    comma = ((attributes->size() > 0) || 
             (event_clock_labels->size() > 0) || 
             (locs->size() > 0)
            );
    trace_plocations(plocs, comma);
    os << std::endl;

    // Log callback values
    os << std::endl << "  // Log callback values" << std::endl;
    comma = ((attributes->size() > 0) || 
             (event_clock_labels->size() > 0) || 
             (locs->size() > 0) || 
             (plocs->size() > 0)
            );
    trace_callbackvalues(uv, comma);
    os << std::endl;

    os << "  // Send the logged state to stdout" << std::endl;
    os << "  outputstate << \")\" << \" : \"" << ";" << std::endl;
    os << "  outputstate << sc_time_stamp() ;" << std::endl;
    os << "  outputstate <<" << " \" ] #\\n\" << std::endl;" << std::endl;

    os << "  std::cout << outputstate.str();" << std::endl;

    if (gp->get_write_to_file()) {
      // Log to tracefile too
      os << "  // Log to tracefile too" << std::endl;
      os << "  *tracefile << outputstate.str();" << std::endl;
      os << std::endl;

      // Delete tracefile pointer
      os << "  // Delete tracefile pointer" << std::endl;
      os << "  if (tracefile) {" << std::endl 
         << "    tracefile->close();" << std::endl  
         << "    delete tracefile;" << std::endl 
          << "  }" << std::endl; 
    }
    
    // end 
    os << "}" << std::endl;
  }
  
  // release the pointer
  delete event_clock_labels;
}


/*
 * Generates the callback code for registering each monitor with the
 * observer
 */
void mon_gen::observer_callbacks() {
  
  // Insert the appropriate callback
  os << std::endl << std::endl;
  
  // virtual void callback_event_notified(sc_event* event);

  // Event clocks
  sset_t* clocks_set = mp->get_clocks();
  sset_t* event_clock_labels = gp->get_event_clock_labels_subset(clocks_set);

  std::string fun_name = "step()";
  
  header_os << std::endl;
  header_os << "  /*" << std::endl;
  header_os << "   * Kernel-level callbacks and user-code callbacks" << std::endl;
  header_os << "   */" << std::endl << std::endl;

  if (event_clock_labels->size() > 0) {
    header_os << "  virtual void callback_event_notified(sc_core::sc_event* event);" << std::endl;
    
    os << "void" << std::endl;
    os << this_monitor_name << "::callback_event_notified(sc_core::sc_event* event) {" << std::endl;

    for (sset_t::iterator it = event_clock_labels->begin(); it != event_clock_labels->end(); ++it) {
      std::string clock_label = *it;
      std::string* event_clock = gp->get_event_clock(&clock_label);
      std::string* event_name = get_event_from_clock(event_clock);
    
      os << "  if (event == &(" << *event_name << ")) {" << std::endl;
      os << "    " << clock_label << " = true;" << std::endl;
      os << "  }" << std::endl << std::endl;

      delete event_clock;
      delete event_name;
    }

    os << "  " << fun_name << ";" << std::endl << std::endl;

    for (sset_t::iterator it = event_clock_labels->begin(); it != event_clock_labels->end(); ++it) {
      std::string clock_label = *it;
      std::string* event_clock = gp->get_event_clock(&clock_label);
      std::string* event_name = get_event_from_clock(event_clock);
    

      os << "  if (event == &(" << *event_name << ")) {" << std::endl;
      os << "    " << clock_label << " = false;" << std::endl;
      os << "  }" << std::endl << std::endl;

      delete event_clock;
      delete event_name;
    }

    os << "}" << std::endl << std::endl;
  }

  delete event_clock_labels;
  // End event clocks

  for (sset_t::iterator it = clocks_set->begin();
      it != clocks_set->end();
      ++it) {
      std::string clk = *it;
      bool is_kernel_clock = (strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0);
      
      // Here we handle the kernel callbacks
      for (unsigned int i = 0; i < kernel_clocks.size(); i++) {
         if (strcasecmp(clk.c_str(), kernel_clocks[i].c_str()) == 0) {
             is_kernel_clock = true;
         }
      }

      //Event
      bool is_event = gp->is_event_clock(&clk);
      //End Event

      // Either kernel clock, event clock or default clock
      if (is_kernel_clock) {
         if ((strcasecmp(clk.c_str(), "MON_INIT_PHASE_BEGIN") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_phase_begin();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_phase_begin() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;      
         }

        if ((strcasecmp(clk.c_str(), "MON_INIT_PHASE_END") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_phase_end();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_phase_end() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;
        }

        if ((strcasecmp(clk.c_str(), "MON_INIT_UPDATE_PHASE_BEGIN") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_update_phase_begin();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_update_phase_begin() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;      
        }

        if ((strcasecmp(clk.c_str(), "MON_INIT_UPDATE_PHASE_END") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_update_phase_end();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_update_phase_end() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;   
        } 

        if ((strcasecmp(clk.c_str(), "MON_INIT_DELTA_NOTIFY_PHASE_BEGIN") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_delta_notify_phase_begin();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_delta_notify_phase_begin() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;
        }


        if ((strcasecmp(clk.c_str(), "MON_INIT_DELTA_NOTIFY_PHASE_END") == 0) || 
            strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
            header_os << "  virtual void callback_init_delta_notify_phase_end();" << std::endl;
            os << "void" << std::endl;
            os << this_monitor_name << "::callback_init_delta_notify_phase_end() {" << std::endl;
            os << "  " << fun_name << ";" << std::endl;
            os << "}" << std::endl << std::endl;
        }


        if ((strcasecmp(clk.c_str(), "MON_DELTA_CYCLE_BEGIN") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_delta_cycle_begin();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_delta_cycle_begin() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_DELTA_CYCLE_END") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_delta_cycle_end();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_delta_cycle_end() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }

        if ((strcasecmp(clk.c_str(), "MON_EVALUATION_PHASE_BEGIN") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_evaluation_phase_begin();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_evaluation_phase_begin() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_EVALUATION_PHASE_END") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_evaluation_phase_end();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_evaluation_phase_end() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }
    

        if ((strcasecmp(clk.c_str(), "MON_UPDATE_PHASE_BEGIN") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_update_phase_begin();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_update_phase_begin() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_UPDATE_PHASE_END") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_update_phase_end();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_update_phase_end() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }

        if ((strcasecmp(clk.c_str(), "MON_DELTA_NOTIFY_PHASE_BEGIN") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_delta_notify_phase_begin();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_delta_notify_phase_begin() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_DELTA_NOTIFY_PHASE_END") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_delta_notify_phase_end();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_delta_notify_phase_end() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_TIMED_NOTIFY_PHASE_BEGIN") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_timed_notify_phase_begin();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_timed_notify_phase_begin() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_TIMED_NOTIFY_PHASE_END") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_timed_notify_phase_end();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_timed_notify_phase_end() {" << std::endl;
          os << " " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_METHOD_SUSPEND") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_method_suspend();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_method_suspend() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }


        if ((strcasecmp(clk.c_str(), "MON_THREAD_SUSPEND") == 0) || 
          strcasecmp(clk.c_str(), "DEFAULT_CLOCK") == 0) {
          header_os << "  virtual void callback_thread_suspend();" << std::endl;
          os << "void" << std::endl;
          os << this_monitor_name << "::callback_thread_suspend() {" << std::endl;
          os << "  " << fun_name << ";" << std::endl;
          os << "}" << std::endl << std::endl;      
        }
      }

      // It is not a kernel clock, nor the DEFAULT_CLOCK
      else if (!is_event) {
        // It is not a kernel clock (including event clocks), nor the DEFAULT_CLOCK
        // User-defined clock (e.g., function entry)
  
        header_os << "  void callback_userloc_" << clk << "();" << std::endl;
        os << "void" << std::endl;
        os << this_monitor_name << "::callback_userloc_" << clk << "() {" << std::endl;
        os << std::endl << "  #ifdef DEBUG_USERLOCS" << std::endl;
        os << "    std::cout << \"" << this_monitor_name << "::" << clk << " callback"
           << "\" << std::endl << std::endl;" << std::endl;
        os << "  #endif" << std::endl << std::endl;
        os << "  // Set the value of the primitive" << std::endl;
        os << "  " << clk << " = true;" << std::endl << std::endl;
        os << "  " << fun_name << ";" << std::endl << std::endl;
        os << "  // Reset the value of the primitive" << std::endl;
        os << "  " << clk << " = false;" << std::endl;
        os << "}" << std::endl << std::endl << std::endl;      
      }
    }
}

/*
 * Generates the callbacks with values of variables in the user code.
 */
void mon_gen::generate_value_callbacks() {

  std::vector<user_val_t*>* uv = gp->get_user_vals();

  header_os << std::endl << std::endl
            << "  // Value callbacks (if any)" << std::endl;
  
  for (unsigned int i = 0; i < uv->size(); i++) {
    user_val_t* this_uv = uv->at(i);
    std::stringstream ss;
    ss << "value_callback_" << this_uv->val_name
       << "(" << this_uv->val_type;

    std::string actual_param_name;
    // Avoid variable name clashes
    if (this_uv->val_name.compare("val") == 0) {
      actual_param_name = "value";
    }
    else {
      actual_param_name = "val";
    }

    ss << " " << actual_param_name << ")";
    
    header_os << "  void " << ss.str() << ";" << std::endl;

    os << "void" << std::endl;
    os << this_monitor_name << "::" << ss.str() << " {" << std::endl;
    os << "  #ifdef DEBUG_USERVALS" << std::endl;
    os << "    std::cout << \"" << this_monitor_name << "::" << ss.str() << ":\"" << std::endl;
    os << "              << \"The value passed is \" << " << actual_param_name << " << std::endl << std::endl;" << std::endl;
    os << "  #endif" << std::endl;
    os << "  " << this_uv->val_name << " = "
       << actual_param_name << ";" << std::endl;
    os << "}" << std::endl << std::endl;
  }
}


/*
 * The class variables used by the monitor.
 */
void mon_gen::start_declaring_monitor_class_variables() {

  header_os << std::endl << std::endl;
  header_os << "  /*" << std::endl;
  header_os << "   * Attributes" << std::endl;
  header_os << "   */" << std::endl << std::endl;

  header_os << "private: " << std::endl << std::endl;
  
  header_os << "  sc_core::mon_observer* observer;" << std::endl;
  header_os << "  mon_status_t status;" << std::endl;

  for (ssmap_t::const_iterator it = gp->get_vartypes()->begin(); 
       it != gp->get_vartypes()->end();
       ++it ) 
    {
      header_os << "  " << it->second << " " << it->first << ";" << std::endl;
    }

  // Declare the location primitives
  std::vector<func_loc_t*>* locs = gp->get_func_locs();
  for (unsigned int i = 0; i < locs->size(); i++) {
    header_os << "  bool " << locs->at(i)->loc_name << ";" << std::endl;
  }

  //plocation
  // Declare the plocation primitives
  std::vector<plocation_t*>* plocs = gp->get_plocations();
  for (unsigned int i = 0; i < plocs->size(); i++) {
    header_os << "  bool " << plocs->at(i)->loc_name << ";" << std::endl;
  }

  // Declare the user value primitives
  std::vector<user_val_t*>* uv = gp->get_user_vals();

  for (unsigned int i = 0; i < uv->size(); i++) {
    user_val_t* this_uv = uv->at(i);
    header_os << "  " << this_uv->val_type << " "
        << this_uv->val_name << ";" << std::endl;
  }

  // Module attributes - no need to declare
  
  // Event clocks variables
  sset_t* clocks_set = mp->get_clocks();
  sset_t* event_clock_labels = gp->get_event_clock_labels_subset(clocks_set);

  for (sset_t::iterator it = event_clock_labels->begin(); it != event_clock_labels->end(); ++it) {
    std::string clock_label = *it;
  
    header_os << "  bool " << clock_label << ";" << std::endl;
  }

  /* Don't need any more
  // Trace length
  header_os << "  unsigned int tracelength;" << std::endl;
  // The number of states
  header_os << "  unsigned int numberofstates;" << std::endl;
  // Trace file name
  header_os << "  char* tracefilename;" << std::endl;
  // Trace file mode
  header_os << "  bool freshtrace;" << std::endl;
  */
}


void mon_gen::finish_declaring_monitor_class_variables() {
  header_os << "};" << std::endl << std::endl;
}

