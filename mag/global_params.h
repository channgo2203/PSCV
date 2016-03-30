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


#ifndef GLOBAL_PARAMS_H
#define GLOBAL_PARAMS_H

#include <iostream>
#include <cassert>
#include <fstream>
#include <string.h>

#include "types.h"
#include "monitor_params.h"
#include "utils.h"


class global_params {
  
 public:

  // Constructor
  global_params(int argc, char** argv);
  
  // Destructor
  ~global_params();
  
  void set_conf_file(char* fn) {  conf_file = std::string(fn); }
  
  void set_verbosity(unsigned int v) { verbosity = v; }
  unsigned int get_verbosity() { return verbosity; }
  
  void set_output_file(char* file_name) {output_file = std::string(file_name);}
  void set_header_output_file(char* file_name) {header_output_file = std::string(file_name);}
  const char* get_output_file() {return output_file.c_str(); }
  const char* get_header_output_file() {return header_output_file.c_str(); }  
  
  void add_includefile(std::string fn) { includefiles.insert(fn); }
  void add_userobjects(std::string fn) { user_objects.insert(fn); }
  
  sset_t get_includefiles() { return includefiles; }
  void includefiles_to_stream(std::ostream &os);
  
  sset_t get_userobjects() { return user_objects; }
  
  void set_mon_name(std::string mn) {mon_name = mn;}
  const char* get_mon_name() {return mon_name.c_str();}
  
  void add_mp(monitor_params* mp) {all_monitors.insert(mp);}
  
  void add_vartype(std::string a, std::string b) {all_objects[a] = b;}
  ssmap_t* get_vartypes() {return &all_objects;}

  void add_attributelabel(std::string a, std::string b) {all_attributelabels[a] = b;}
  ssmap_t* get_attributelabels() {return &all_attributelabels;}
  void add_attributetype(std::string a, std::string b) {all_attributetypes[a] = b;}
  ssmap_t* get_attributetypes() {return &all_attributetypes;}

  void add_eventclocklabel(std::string a, std::string b) {all_eventclocklabels[a] = b;}
  ssmap_t* get_eventclocklabels() {return &all_eventclocklabels;}
  bool is_event_clock(std::string* label);
  sset_t* get_event_clock_labels_subset(sset_t* labels);
  std::string* get_event_clock(std::string* label);


  void to_stream(std::ostream &os);
  
  void get_scratch_location(std::string* container) { *container = scratch; }
  void get_systemc_home(std::string* sysc ) { *sysc = systemc_home; }

  bool get_write_to_file() { return write_to_file; }

  bool get_plasma_project_only() { return plasma_project_only; }
  
  void add_location(std::string loc);

  // add plocation
  void add_pattern_location(std::string loc);
  std::vector<plocation_t*>* get_plocations() {return &plocations;}
  
  void add_value(std::string val);
  std::vector<func_loc_t*>* get_func_locs() {return &func_locs;}
  std::vector<user_val_t*>* get_user_vals() {return &user_vals;}
  
  // A set of monitor_params objects. Each object contains the params
  // of an individual monitor
  mpset_t all_monitors;

  // A set of objects (and their types) that will need to be passed to
  // the local observer from the top level of the model
  ssmap_t all_objects;

  ssmap_t all_attributelabels;
  
  ssmap_t all_attributetypes;

  ssmap_t all_eventclocklabels;

  // A subset of the types from the set above, which have been defined
  // as a "user" types. We will need forward class declarations for
  // those.
  sset_t user_objects;

  void user_val_to_stream(user_val_t* uv, std::ostream& os = std::cout);
  
  // Pretty print of func_locs 
  void func_loc_to_stream(func_loc_t* loc, std::ostream& os = std::cout);

  // time resolution
  sset_t* get_timeresolution() { return &timeresolution; }
  void set_timeresolution(std::string parsed_formula);

  // plasma project file information
  void set_projectfile(char* file_name) {projectfile = std::string(file_name);}
  const char* get_projectfile() {return projectfile.c_str();}

  void set_projectname(std::string mn) {projectname = mn;}
  const char* get_projectname() {return projectname.c_str();}
  
  void set_modelname(std::string mn) {modelname = mn;}
  const char* get_modelname() {return modelname.c_str();}

  void set_modeltype(std::string mn) {modeltype = mn;}
  const char* get_modeltype() {return modeltype.c_str();}

  void set_modelcontent(char* file_name) {modelcontent = std::string(file_name);}
  const char* get_modelcontent() {return modelcontent.c_str();}

  void set_requirementtype(std::string mn) {requirementtype = mn;}
  const char* get_requirementtype() {return requirementtype.c_str();}


 protected:
  
  // Parse data from the configuration file
  void parse_config_file(unsigned int);

  // Parse data from the command line
  void parse_command_line(int, char**);

  // Print help and exit
  void help(char* prog_name);
  
  // Print the contents of the ssmap_t to the output stream os
  void ssmap_to_stream(ssmap_t my_map, std::ostream& os);
  
  // Print the contents of the mpset_t to the output stream os
  void mpset_to_stream(mpset_t my_set, std::ostream& os);

  // Extract and set the name of the output file
  void set_output_file(std::string name);

  // Extract and set plasma file
  void set_projectfile(std::string name);

  // Extract and set plasma model content
  void set_modelcontent(std::string name);

  // Extract and set the name of the output header file
  void set_header_output_file(std::string name);

  // Extract and set the write to file flag
  void set_write_to_file(std::string name);

  // Extract and set the generate only plasma project file flag
  void set_plasma_project_only(std::string name);
  
  // Pretty print the user values
  void user_vals_to_stream(std::ostream& os);
  
  const char* funcbefter2str(func_befter_t fb);
  void func_locs2stream(std::ostream& os = std::cout);

  void plocations2stream(std::ostream& os = std::cout);

  // Module attributes print
  void attributelabels_to_stream(std::ostream& os);
  void attributetypes_to_stream(std::ostream& os);

  // Event clock print
  void eventclocklabels_to_stream(std::ostream& os);

  // Sanity checks for the input parameters
  void sanity_checks();

  // The location of the configuration file
  std::string conf_file;

  // How much talking we do. The lower, the quieter
  unsigned int verbosity;

  // The name to give to the class of generated monitors.
  std::string mon_name;

  // Files to be included in the monitor definition file
  sset_t includefiles;

  // The file name where all monitors and other objects will go
  std::string output_file;

  // The header file for the monitors
  std::string header_output_file;

  // The "scratch" directory
  std::string scratch;

  // The location of the SystemC root directory
  std::string systemc_home;

  // User-defined locations in the model code
  std::vector<func_loc_t*> func_locs;

  // User-defined pattern locations
  std::vector<plocation_t*> plocations;
 
  // User-defined values from the model code
  std::vector<user_val_t*> user_vals;

  // Write traces to file
  bool write_to_file;

  // Time resolution
  sset_t timeresolution; // Set of all clock expressions. If no clock specified by user, it contains {"DEFAULT CLOCK"}

  // plasma project file information
  std::string projectfile;
  std::string projectname;
  std::string modelname;
  std::string modeltype;
  std::string modelcontent;
  std::string requirementtype;
  bool plasma_project_only; // only generate plasma project file
}; // class

#endif
