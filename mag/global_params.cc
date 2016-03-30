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

#include "global_params.h"

//This is the constructor for a large class, which holds all configuration information and gets passed everywhere
// * sets configuration parameters
// * parses command-line parameters and replaces defaults with any input values
//
// Constructor
global_params::global_params(int argc, char** argv) {

  if (argc == 2 && (strcasecmp("-help", argv[1]) == 0)) {
    help(argv[0]);
    exit(0);
  } //end if
    
  // Set the default values
  conf_file = std::string("mm_config.txt");
  verbosity = 1;
  mon_name = "monitor";
  output_file = "monitor.cc";
  header_output_file = "";
  // write the execution traces to file
  write_to_file = false;
  // generate only plasma project file
  plasma_project_only = false;
  // plasma project file information
  projectfile = "test.plasma";
  projectname = "monitor";
  modelname = "monitor_model";
  //modeltype = "fr.inria.plasmalab.systemc";
  modeltype = "systemc"; // adapt to new version of Plasma lab
  //requirementtype = "fr.inria.plasmalab.bltl";
  requirementtype = "bltl"; // adapt to new version of Plasma lab

  scratch = std::string("/tmp");
  // Setting relative path of systemC
  systemc_home = std::string("../systemc-2.3.0_modified");
  
  // Length of configuration file
  unsigned int conf_length;

  // Parse the command line parameters to get the config file name (if
  // different from default)
  for (int i = 1; i < argc; i = i + 2) {
    if ( strcasecmp(argv[i], "-conf") == 0 ) {
      set_conf_file( argv[i+1] );
    } //end if
  } //end for

  if (conf_file.size() > 0) {
    // Determine the length of the configuration file
    std::ifstream is( conf_file.c_str(), std::ios::binary );
    if (! is) {
      std::cerr << "ERROR: Configuration file \'" << conf_file
		            << "\' could not be opened for reading!" << std::endl;
      exit (1);
    } //end if
    
    is.seekg (0, std::ios::end);
    conf_length = is.tellg();
    is.close();
    
    // Now we are ready to parse everything from the config file
    std::cout << "Parsing the configuration file " << std::endl;
    parse_config_file(conf_length);
    std::cout << "Done" << std::endl;
  } //end if


  // Parse the command line parameters (take precedence)
  std::cout << "Parsing the command line parameters " << std::endl;
  parse_command_line(argc, argv);
  std::cout << "Done" << std::endl;   

  std::cout << "Starting sanity checks " << std::endl;
  sanity_checks();
  std::cout << "Done" << std::endl;
} //end global_params constructor


global_params::~global_params() {
  // release memory of all monitors
  for (mpset_t::iterator it = all_monitors.begin(); it != all_monitors.end(); ++it) {
    monitor_params* mp = *it;
    delete mp;
  }
}


/*
 * Check the values of the parsed parameters to ensure consistency
 */
void global_params::sanity_checks() {

  if (strlen(get_output_file()) == 0) {
    std::cerr << "You must provide the name of the output file!" << std::endl;
    exit (1);
  } //end if

  if (all_attributelabels.size() != all_attributetypes.size()) {
        std::cerr << "Your attribute definitions are not correct!" << std::endl;
        exit (1);
  } //end if
  
  // check the equivalence between the number of requirements 
  // and number of formulas

  if (header_output_file.size() == 0) {
    std::cerr << "Warning: No header output file defined! " << std::endl;
    std::string::size_type dot_location = output_file.find_last_of(".");

    if (dot_location == std::string::npos) {
      // There is no dot in the file name??
      header_output_file = output_file;
      header_output_file.append(".h"); 
    } //end if
    else {
      header_output_file = output_file.substr(0, dot_location);
      header_output_file.append(".h");
    } //end else
    
    std::cerr << "Deriving a header file name from the name of the output file: " << header_output_file << std::endl;
      
  } //end if
} //end sanity_checks()


void global_params::parse_command_line(int argc, char** argv) {
  for (int i = 1; i < argc; i = i + 2) {
    if ( strcasecmp(argv[i], "-verbosity") == 0 ) {
      set_verbosity( atoi(argv[i+1]) );
    } //end if

    if ( strcasecmp(argv[i], "-scratch") == 0 ) {
      scratch = std::string(argv[i+1]);
    } //end if

    if ( strcasecmp(argv[i], "-systemc_home") == 0 ) {
      systemc_home = std::string(argv[i+1]);
    } //end if

    if ( strcasecmp(argv[i], "-output_file") == 0 ) {
      set_output_file(argv[i+1]);
    } //end if

    if ( strcasecmp(argv[i], "-header_output_file") == 0 ) {
      set_header_output_file(argv[i+1]);
    } //end if

    // write to file
    if ( strcasecmp(argv[i], "-write_to_file") == 0 ) {
      set_write_to_file(std::string(argv[i+1]));
    }

    // Plasma project file only
    if ( strcasecmp(argv[i], "-plasma_project_only") == 0 ) {
      set_plasma_project_only(std::string(argv[i+1]));
    }

  } //end for
} //end parse_command_line()


void global_params::to_stream(std::ostream &os) {
  os << "Values of parameters:" << std::endl;
  os << "\t configuration file = " << conf_file << std::endl;
  
  os << "\t verbosity = " << verbosity << std::endl;
  os << "\t monitor_name = " << mon_name << std::endl;
  os << "\t output file = " << output_file << std::endl;
  os << "\t header output file = " << header_output_file << std::endl;
  os << "\t write traces to file = " << write_to_file << std::endl;
  os << "\t generate only plasma project file = " << plasma_project_only << std::endl;
  os << "\t plasma project file = " << projectfile << std::endl;
 
  os << "\t scratch = " << scratch << std::endl;
  os << "\t SystemC home = " << systemc_home << std::endl;
  
  os << "\t User-defined locations: " << std::endl;
  func_locs2stream(os);

  // plocation
  os << "\t Pattern-based locations: " << std::endl;
  plocations2stream(os);

  // user-defined values
  os << "\t User-defined values: " << std::endl;
  user_vals_to_stream(os);
  
  // observer
  os << "\t Observer objects:  " << std::endl;
  ssmap_to_stream(all_objects, os);

  // user-defined objects
  os << "\t User-defined objects: " << std::endl;
  for (sset_t::const_iterator it = user_objects.begin();
    it != user_objects.end();
    it++) {
    std::cout << "\t\t" << *it << std::endl;
  } //end for

  // module attribute labels
  os << "\t User-defined attributes: " << std::endl;
  attributelabels_to_stream(os);
    
  // attributetypes
  os << "\t User-defined attribute types: "<< std::endl;
  attributetypes_to_stream(os);
    
  // event clock labels
  os << "\t User-defined event clocks: " << std::endl;
  eventclocklabels_to_stream(os);

  // monitor parameters
  os << "\t Individual monitor params:  " << std::endl;
  mpset_to_stream(all_monitors, os);
}


/*
 * Formats and prints the includefiles as they should appear in the
 * generated output file
 */
void global_params::includefiles_to_stream(std::ostream &os) {
  for (sset_t::iterator it = includefiles.begin();
    it != includefiles.end();
    ++it ) { 
    
    if (it->find('<') != std::string::npos) {
      // Do not quote includes that contain "<>"
	    assert(it->find('>') != std::string::npos);
	    os << "#include " << *it << std::endl;
    } //end if
    
    else if (it->find('"') != std::string::npos) {

	     //... or contain quotes already
	     os << "#include " << *it << std::endl;
    } //end else if
    
    else {
	     os << "#include \"" << *it << "\"" << std::endl;

    }
  }
}


/*
 * Parse the configuration file for parameters
 */
void global_params::parse_config_file(unsigned int conf_length) {
  
  std::ifstream config_f(conf_file.c_str(), std::ios::in);
  
  std::string p, v;
  while (config_f >> p) {
  
    #ifdef DEBUG_GLOBAL_PARAMS
      std::cout << "Read \"" << p << "\" from the configuration file." << std::endl;
    #endif

    if (p == "verbosity") {
      if (config_f >> v) {
	       set_verbosity( atoi(v.c_str()) );
      } //end if
      
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end if

    else if (p == "mon_name") {
      if (config_f >> v) {
	       set_mon_name(v);
      } //end if

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    } //end else if
    
    else if (p == "systemc_home") {
      if (config_f >> v) {
	       systemc_home = v;
      } //end if
      
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    } //end else if

    else if (p == "scratch") {
      if (config_f >> v) {
	       scratch = v;
      } //end if

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    } //end else if
    
    else if (p == "usertype") {
      std::string first;
      config_f >> first;
      add_userobjects(first);

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else if
    
    else if (p == "output_file") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Unable to extract the output_file " 
		               << "from configuration file" << conf_file << std::endl;
      } //end if
      else {
	       set_output_file(v);
      }
    }

    // only generate plasma project file
    else if (p == "plasma_project_only") {
      std::string first;
      config_f >> first;
      set_plasma_project_only(first);
      
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    }

    // plasma file location
    else if (p == "plasma_file") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
         std::cerr << "ERROR: Unable to extract the output_file " 
                   << "from configuration file" << conf_file << std::endl;
      } //end if
      else {
         set_projectfile(v);
      }
    }

     // plasma project name
    else if (p == "plasma_project_name") {
      if (config_f >> v) {
         set_projectname(v);
      } //end if

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    } //end else if

    // plasma model name
    else if (p == "plasma_model_name") {
      if (config_f >> v) {
         set_modelname(v);
      } //end if

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    } //end else if

    // plasma model content
    else if (p == "plasma_model_content") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
         std::cerr << "ERROR: Unable to extract the output_file " 
                   << "from configuration file" << conf_file << std::endl;
      } //end if
      else {
         set_modelcontent(v);
      }
    }

    // write to file
    else if (p == "write_to_file") {
      std::string first;
      config_f >> first;
      set_write_to_file(first);
      
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n'); 
    }// end else if

    else if (p == "header_output_file") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Unable to extract the header_output_file " 
		               << "from configuration file" << conf_file << std::endl;
      } //end if
      else {
	       set_header_output_file(v);
      }
    }
    
    else if (p == "location") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Empty location definition in configuration file " 
	                 << conf_file << std::endl;
      } //end if
      else {
	       add_location(v);
      }
    }

    // add plocation
    else if (p == "plocation") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Empty pattern-based location definition in configuration file "
	                 << conf_file << std::endl;
      }
      else {
	       add_pattern_location(v);
      }
    }

    else if (p == "value") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Empty variable definition in configuration file " 
		               << conf_file << std::endl;
      }
      else {
	       add_value(v);
      }
      // We already consumed the whole line
    }

    // Modules' attribute labels
    else if (p == "attribute") {
      std::string first, second;
      config_f >> first;
      
      config_f >> second;
      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  attribute first = \"" << first << "\", and attribute second=\"" << second << "\"." << std::endl;
      #endif
      //mp->add_vartype(second, first);
      add_attributelabel(second, first);

      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  attribute moving on." << std::endl;
      #endif

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    }

    // Event clock labels
    else if (p == "eventclock") {
      std::string first, second;
      config_f >> first;
      
      config_f >> second;
      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  eventclock first = \"" << first << "\", and eventclock second=\"" << second << "\"." << std::endl;
      #endif
      //mp->add_vartype(second, first);
      add_eventclocklabel(second, first);

      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  eventclock moving on." << std::endl;
      #endif

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    }
    
    // Time resolution - CLOCK || CLOCK || CLOCK ...
    else if (p == "time_resolution") {
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
         std::cerr << "ERROR: Unable to extract formula from configuration file " 
                   << conf_file << std::endl;
      } //end if
      else {
        // set clocks
        set_timeresolution(v);
      } //end else
    }

    // BLTL formulas
    else if (p == "formula") {
      monitor_params* mp = new monitor_params();
      getline(config_f, v);

      // only continue if characters were extracted
      if (config_f.fail()) {
	       std::cerr << "ERROR: Unable to extract formula from configuration file " 
	                 << conf_file << std::endl;
      } //end if
      else {
        // set formula
	      //mp->set_formula(v);
        mp->add_formula(v);
      } //end else

      add_mp(mp);
      // We already consumed the whole line
    } //end else if

    else if (p == "type") {
      std::string first, second;
      config_f >> first;
      
      config_f >> second;
      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  type first = \"" << first << "\", and type second=\"" << second << "\"." << std::endl;
      #endif
      //mp->add_vartype(second, first);
      add_vartype(second, first);

      #ifdef DEBUG_GLOBAL_PARAMS
        std::cout << "  type moving on." << std::endl;
      #endif

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else if
    
    else if (p == "att_type") {
      std::string first, second;
      config_f >> first;
        
      config_f >> second;
      #ifdef DEBUG_GLOBAL_PARAMS
      std::cout << "  att_type first = \"" << first << "\", and att_type second=\"" << second << "\"." << std::endl;
      #endif
      add_attributetype(second, first);
        
      #ifdef DEBUG_GLOBAL_PARAMS
      std::cout << "  att_type moving on." << std::endl;
      #endif
        
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else if
      
    else if (p == "include") {
      std::string first;
      config_f >> first;
      add_includefile(first);

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else if

    else if (p[0] == '#') {
      // skip over comments

      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else if

    else { 
      std::cout << "Unknown parameter: " << p << " in the config file." << std::endl;
      // Ignore the rest of the line
      config_f.ignore(conf_length, '\n');
    } //end else
  } //end while

  config_f.close();
}  //end parse_config_file


/*
 * Print a helpful message and exit
 */
void global_params::help(char* prog_name) {
  std::cout << "Usage: " << prog_name << " [ PARAMETERS ]" << std::endl;
  std::cout << "-conf                  path to configuration file (./mm_config)]" << std::endl;
  std::cout << "                       You can set all options (except -conf) using the" << std::endl;
  std::cout << "                       configuration file or the command-line switches below." << std::endl;
  
  std::cout << "-verbosity             verbosity of debugging output (3)]" << std::endl;  
  
  std::cout << "-scratch               path to a directory for temporary files (/tmp)" << std::endl;
 
  std::cout << "-output_file file      name (with path) of the generated monitor file" << std::endl;
 
  std::cout << "-header_output_file    file name (with path) of the header file" << std::endl;

  std::cout << "-write_to_file         write the execution traces to a file" << std::endl;  
 
  std::cout << "-plasma_project_only   only generate Plasma project file" << std::endl;
  std::cout << std::endl << std::endl;
}


/*
 * Print a map of strings -> string
 */
void global_params::ssmap_to_stream(ssmap_t my_map, std::ostream& os) {
  for (ssmap_t::const_iterator it = my_map.begin();
    it != my_map.end();
    it ++) {
    os << "\t\t" << it->first << " of type " << it->second << std::endl;
  }
}


/*
 * Print a set of monitor_params objects
 */
void global_params::mpset_to_stream(mpset_t my_set, std::ostream& os) {
  for (mpset_t::const_iterator it = my_set.begin();
    it != my_set.end();
    it++) {
    (*it)->to_stream(os);
  }
}


/*
 * Clean up and set the name of the header output file
 */
void global_params::set_header_output_file(std::string name) {
  
  copy_without_surrounding_whitespace(&name, &header_output_file);
}


/* 
 * Set the flag to write or not to a file
 */
void global_params::set_write_to_file(std::string name) {
  if (strcmp(name.c_str(), "true") == 0) {
    write_to_file = true;
  } 
  // by default set it to be false
  else {
    write_to_file = false;
  }
}


/* 
 * Set the flag to generate only plasma project file
 */
void global_params::set_plasma_project_only(std::string name) {
  if (strcmp(name.c_str(), "true") == 0) {
    plasma_project_only = true;
  } 
  // by default set it to be false
  else {
    plasma_project_only = false;
  }
}


/*
 * Clean up and set the name of the output file
 */
void global_params::set_output_file(std::string name) {
  
  copy_without_surrounding_whitespace(&name, &output_file);
}

/*
 * Clean up and set the name of the output file
 */
void global_params::set_projectfile(std::string name) {
  
  copy_without_surrounding_whitespace(&name, &projectfile);
}


/*
 * Clean up and set the name of the output file
 */
void global_params::set_modelcontent(std::string name) {
  
  copy_without_surrounding_whitespace(&name, &modelcontent);
}


/*
 * set the time resolution
 */
void global_params::set_timeresolution(std::string parsed_formula) {
  // Remove leading whitespace
  char const* clock_delims = " \t\f\v\n\r|";
  std::string::size_type notwhite = parsed_formula.find_first_not_of(clock_delims);

  // Empty clocks
  if (notwhite == std::string::npos) {
    timeresolution.insert(std::string("DEFAULT_CLOCK"));
  }
  // Some clocks are given
  else {
    parsed_formula.erase(0,notwhite); 

    // Remove trailing garbage
    notwhite = parsed_formula.find_last_not_of(clock_delims);
    
    // There must be nonwhite characters, otherwise we would have
    // taken the other branch of the conditional
    assert(notwhite != std::string::npos);

    parsed_formula.erase(notwhite + 1);

    // loop though all sub clocks
    unsigned long int pos = 0;
    while (pos != std::string::npos) {
      std::string::size_type white = parsed_formula.find_first_of(clock_delims, pos);

      if (white == std::string::npos) {
        // There is no white space, meaning there is only 1 clock
        timeresolution.insert(parsed_formula.substr(pos));
        // Reset the loop's flag
        pos = white;
      }
      else {
        timeresolution.insert(parsed_formula.substr(pos, white - pos));
        pos = parsed_formula.find_first_not_of(clock_delims, white);
      }
    }    
  }
 }


/*
 * Parse the location definition and store it in the set locdefs
 */
void global_params::add_location(std::string loc) {
  func_loc_t* func_loc = new func_loc_t();
  
  // Remove leading whitespace
  char const* delims = " \t\f\v\r\n";
  std::string::size_type notwhite = loc.find_first_not_of(delims);
  loc.erase(0, notwhite);

  // Get the user-given name of the location
  std::string::size_type white = loc.find_first_of(delims);
  func_loc->loc_name = std::string(loc.substr(0, white));
  loc.erase(0, white);

  // Remove leading whitespace again
  loc.erase(0, loc.find_first_not_of(delims));

  // Find the func_befter
  std::string::size_type colon_loc = loc.find_last_of(":");
  func_loc->func_identifier = loc.substr(0, colon_loc);

  if (loc.find("entry", colon_loc) != std::string::npos) {
    func_loc->func_befter = ENTRY;
  } //end if
  
  else if (loc.find("exit", colon_loc) != std::string::npos) {
    func_loc->func_befter = EXIT;
  } //end else
  
  else if (loc.find("call", colon_loc) != std::string::npos) {
    func_loc->func_befter = CALL;
  } //end else

  else if (loc.find("return", colon_loc) != std::string::npos) {
    func_loc->func_befter = RETURN;
  } //end else

  else if (loc.find("dummy", colon_loc) != std::string::npos) {
    func_loc->func_befter = DUMMY;
  } //end else

  else {
    std::cerr << "Error parsing the function befter from the string \"" << loc << "\"" << std::endl;
    exit(1);
  } //end else

  func_locs.push_back(func_loc);
} //end global_params::add_location


const char* global_params::funcbefter2str(func_befter_t fb) {
  if (fb == ENTRY)
    return "entry";
  if (fb == EXIT)
    return "exit";
  if (fb == CALL)
    return "call";
  if (fb == RETURN)
    return "return";

  assert (fb == DUMMY);
  return "dummy";
}

/*
 * Pretty pring of individual func_locs
 */
void global_params::func_loc_to_stream(func_loc_t* loc, std::ostream& os) {
  os << "\t\t" << loc->loc_name << "="
     << loc->func_identifier << ":"
     << funcbefter2str(loc->func_befter);
}


/*
 * Pretty printing of all func_locs
 */
void global_params::func_locs2stream(std::ostream& os) {
  for (unsigned int i=0; i<func_locs.size(); i++) {
    func_loc_to_stream(func_locs[i], os);
    os << std::endl;
  }
}


/*
 * Parses and stores the user-defined value
 */
void global_params::add_value(std::string val) {
  std::stringstream ss(val);
  std::string vartype, varname;
  std::stringbuf rest;
  
  ss >> vartype >> varname;
  ss.get(rest);
  std::string rest_str(rest.str());
  copy_without_surrounding_whitespace(& rest_str, & rest_str);

  // Find the parameter index
  std::string::size_type colon_pos = rest_str.find_last_of(":");
  std::string index = rest_str.substr(colon_pos + 1);
  int index_int = str2int( index);

  // The rest is the function description
  std::string description = rest_str.substr(0, colon_pos);
  copy_without_surrounding_whitespace(& description, & description);

  user_val_t* uv = new user_val_t;
  uv->val_type = vartype;
  uv->val_name = varname;
  uv->func_identifier = description;
  uv->param_index = index_int;

  user_vals.push_back(uv);
}


void global_params::user_vals_to_stream(std::ostream& os) {
  for (unsigned int i = 0; i < user_vals.size(); i++) {
    this->user_val_to_stream(user_vals[i], os);
    os << std::endl;
  }
}


void global_params::user_val_to_stream(user_val_t* uv, std::ostream& os) {
  os << "\t\t" << uv->val_type
     << " " << uv->val_name
     << " = " << uv->func_identifier
     << " : " << uv->param_index;
}


// Module attributes print
void global_params::attributelabels_to_stream(std::ostream& os) {

  for (ssmap_t::const_iterator it = all_attributelabels.begin();
    it != all_attributelabels.end();
    it ++) {
    os << "\t\t" << it->first << " is " << it->second << std::endl;
  }
}

// Attributetypes print
void global_params::attributetypes_to_stream(std::ostream& os) {
    for (ssmap_t::const_iterator it = all_attributetypes.begin(); 
         it != all_attributetypes.end();
         it ++) {
         os << "\t\t" << it->first << " is " << it->second << std::endl;
    }

}


// Event clock print
void global_params::eventclocklabels_to_stream(std::ostream& os) {

  for (ssmap_t::const_iterator it = all_eventclocklabels.begin();
    it != all_eventclocklabels.end();
    it ++) {
    os << "\t\t" << it->first << " is " << it->second << std::endl;
  }
}


//plocation
/*
 * Parse a pattern-based location definition and store it in the set locdefs
 */
void global_params::add_pattern_location(std::string loc) {

  plocation_t* pattern_loc = new plocation_t();

  // Remove leading whitespace
  char const* delims = " \t\f\v\r\n";
  std::string::size_type notwhite = loc.find_first_not_of(delims);
  loc.erase(0, notwhite);

  // Get the user-given name of the location
  std::string::size_type white = loc.find_first_of(delims);
  pattern_loc->loc_name = std::string(loc.substr(0, white));
  loc.erase(0, white);

  // Remove leading whitespace again
  loc.erase(0, loc.find_first_not_of(delims));

  // Find the pattern
  std::string::size_type quote = loc.find_first_of("\"");
  if (quote == std::string::npos) {
    std::cerr << "*******" << std::endl
	      << "* ERROR: Pattern definition must be in quotes: " << loc << std::endl
	      << "* ERROR: Pattern location " << pattern_loc->loc_name << " will be ignored" << std::endl
	      << "*******" << std::endl;
  }
  else {
    loc.erase(0, quote + 1);
    quote = loc.find_last_of("\"");
    pattern_loc->pattern = loc.substr(0, quote);
    plocations.push_back(pattern_loc);
  }
} //end global_params::add_location
//End plocation


/*
 * Pretty printing of pattern locations
 */
void global_params::plocations2stream(std::ostream& os) {
  for (unsigned int i=0; i<plocations.size(); i++) {
    plocation_t* loc = plocations[i];
    os << "\t\t" << loc->loc_name << " = "
       << "-->" << loc->pattern << "<--"
       << std::endl;
  }
}


bool global_params::is_event_clock(std::string* label) {

  ssmap_t::iterator it;
  std::string evtlbl = *label;
  it = all_eventclocklabels.find(evtlbl);

  if (it != all_eventclocklabels.end()) {
      return true;
  }

  return false;
}


/*
 * Get event clocks from the corresponding labels
 */
sset_t* global_params::get_event_clock_labels_subset(sset_t* labels) {

  sset_t* event_clocks = new sset_t();

  for (sset_t::iterator it = labels->begin(); it != labels->end(); ++it) {
    std::string label = *it;

    if (is_event_clock(&label)) {
      event_clocks->insert(label);
    }
  }

  return event_clocks;
}

/*
 * Get event clock from the corresponding label
 */

std::string* global_params::get_event_clock(std::string* label) {

  return new std::string(all_eventclocklabels.find(*label)->second);
}