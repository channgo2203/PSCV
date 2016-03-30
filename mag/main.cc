/*  
    Copyright (C) 2012 Rice University

    This file is part of CHIMP.
    CHIMP is a free software that instruments a SystemC model
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
    
    File modified by INRIA (Institut National de Recherche en 
    Informatique et Automatique).
*/


#include <iomanip>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "global_params.h"
#include "monitor_params.h"
#include "types.h"
#include "utils.h"


#include "aspect.h"
#include "observer.h"
#include "trace_gen.h"
#include "patterns.h"
#include "plasma_gen.h"


/*
 * Generate the monitors
 */
void generate_monitors(global_params* gp, 
  std::ofstream& out_strm, 
  std::ofstream& out_strm_h) {

  // Copyright
  copyright_generation(out_strm);
  copyright_generation(out_strm_h);

  // Print out the beginning headers of generated monitor code to the output file
  std::string capitalized; //the #ifdef version of the filename
  
  // Include guard
  get_capitalized_name(gp->get_header_output_file(), & capitalized);
  out_strm_h << "#ifndef " << capitalized << std::endl;             
  out_strm_h << "#define " << capitalized << std::endl << std::endl;
  
  // Include systemc header file
  out_strm_h << "#include <systemc.h>" << std::endl;
  
  // Include other header files
  out_strm_h << "#include <iostream>" << std::endl;
  out_strm_h << "#include <fstream>"  << std::endl;
  out_strm_h << "#include <cassert>"  << std::endl;   
  out_strm_h << "#include <cstdlib>"  << std::endl;
  out_strm_h << "#include <sstream>"  << std::endl;
  out_strm_h << "#include <string.h>" << std::endl << std::endl;

  // Debug flags
  //out_strm_h << "#define TIMING" << std::endl;
  //out_strm_h << "#define DEBUG_UTILS" << std::endl;
  //out_strm_h << "#define DEBUG_GLOBAL_PARAMS" << std::endl;
  //out_strm_h << "#define DEBUG_TRACE_GEN" << std::endl;
  //out_strm_h << "#define DEBUG_USERVALS" << std::endl;
  //out_strm_h << "#define DEBUG_USERLOCS" << std::endl << std::endl;

  // User's includes to the monitor's cc file
  out_strm << "/*" << std::endl;
  out_strm << " * User's includes to cc file" << std::endl;
  out_strm << " */" << std::endl << std::endl;
  gp->includefiles_to_stream(out_strm);

  // Header file include
  std::string naked_name; // the plain file name, stripped of the path
  get_naked_file_name(gp->get_header_output_file(), & naked_name);
  out_strm << "#include \"" << naked_name << "\"" << std::endl << std::endl;
  
  // Declare each user object as a class in output header file
  out_strm_h << "/*" << std::endl;
  out_strm_h << " * User objects, forward declaration" << std::endl;
  out_strm_h << " */" << std::endl << std::endl;
  for (sset_t::const_iterator it = gp->user_objects.begin(); 
    it != gp->user_objects.end(); 
    it++) {
    std::string user_object = *it;
    declare_forward(&user_object, out_strm_h);
  }
  out_strm_h << std::endl;
  
  // Generate the monitor
  if (gp->all_monitors.size() > 0) {
    std::cout << "Starting to generate a monitor ..." << std::endl;
    mpset_t::const_iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;
    trace_generation(gp, mp, out_strm, out_strm_h);
    std::cout << "Done generating a monitor" << std::endl;
  }

  // Loop through all monitors
  //int i = 0;
  //for(mpset_t::const_iterator it = gp->all_monitors.begin(); 
  //  it != gp->all_monitors.end(); 
  //  ++it) { 
    
  //  std::cout << "Starting to generate a monitor " << i << "..." << std::endl;
  //  Monitor parameters
  //  monitor_params* mp = *it;
  //  trace_generation(gp, mp, out_strm, out_strm_h);
  //  std::cout << "Done generating a monitor " << i << std::endl;
  //  i++;
  //}  
}


/*
 * Generate plasma project file - xml format
 */
void generate_plasma_file(global_params* gp) {

   plasma_gen plasma_gernerator = plasma_gen(gp);
   plasma_gernerator.generate_plasma_file();
}


// Entry of program
int main(int argc, char** argv) {
  std::string version = "1.0";

  #ifdef TIMING
	start_final_timer();
  #endif

  // Tool's information
  std::cout << "-------------------" << std::endl;
  std::cout << "MAG v" << version << std::endl;
  std::cout << "Copyright (C) 2014 Van-Chan Ngo, INRIA" << std::endl;
  std::cout << "-------------------" << std::endl;

  // Parse command line inputs and set configuration
  global_params* gp = new global_params(argc, argv); // calls global_params.cc:global_params::global_params(int argc, char** argv)

  if (gp->get_plasma_project_only()) {
    // Print the values of all configuration parameters to cout
    gp->to_stream(std::cout);
    // Generate the plasma project file
     generate_plasma_file(gp);
  }
    
  else {
    // Set clocks for each monitor_param
    sset_t* t = gp->get_timeresolution();
    for(mpset_t::const_iterator it = gp->all_monitors.begin(); 
      it != gp->all_monitors.end();
      ++it) { 
    
      monitor_params* mp = *it;
      mp->set_timeresolution(*t);
    }  
  
    // Print the values of all configuration parameters to cout
    gp->to_stream(std::cout); // global_params.cc:global_params::to_stream

    // Open two output files, check for errors, and assign the file handles
    std::ofstream& out_strm = *open_for_output(gp->get_output_file());
  
    // global_params.h:get_output_file returns the filename as a string
    std::ofstream& out_strm_h = *open_for_output(gp->get_header_output_file());
  
    // The vast majority of the program is triggered by the following function
    generate_monitors(gp, out_strm, out_strm_h);
  
    // Generate the local observer at the end 
    generate_local_observer(out_strm, out_strm_h, gp);

    // Generate an aspect file
    generate_aspect_file(gp); //aspect.cc

    // Generate regular expression - pattern for plocation
    generate_patterns_file(gp);

    // Generate plasma project file - xml format
    generate_plasma_file(gp);

    out_strm_h << std::endl << "#endif" << std::endl;
  
    // Close output files 
    close_file(&out_strm); 
    close_file(&out_strm_h);
  }

  #ifdef TIMING
  stop_final_timer();
  #endif

  // Release global params pointer
  delete gp;
  
  // End of program
  return 0;
}


