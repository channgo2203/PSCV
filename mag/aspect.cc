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

#include "aspect.h"


/**
 * Generates the advice that declares all monitor classes as friends.
 */
void
generate_friendship(std::ofstream& os, global_params* gp) {
  if (gp->user_objects.size() == 0) {
    return;
  } //end if

  // First define which classes will declare us friends 
  os << std::endl << std::endl
     << "  // Declare the monitor objects friends, for access to private data" << std::endl;
  os << "  pointcut reveal() = ";
  for (sset_t::const_iterator it = gp->user_objects.begin();
       it != gp->user_objects.end();
       it++) {
    if (it != gp->user_objects.begin()) {
      os << " || ";
    } //end if
    
    os << "\"" << *it << "\"";
  } //end for
  os << ";" << std::endl << std::endl;
  
  // Advice definition starts here
  os << "  advice reveal() : slice class {" << std::endl;
  
  if (gp->all_monitors.size() > 0) {
    mpset_t::const_iterator it = gp->all_monitors.begin();
    std::string id;
    (*it)->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    friend class " << this_monitor_name << ";" << std::endl;  
  }

  /*
  for (mpset_t::const_iterator it = gp->all_monitors.begin();
       it != gp->all_monitors.end();
       it ++) {
    std::string id;
    (*it)->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    friend class " << this_monitor_name << ";" << std::endl;
  } //end for
  */

  os << "  }; // class" << std::endl << std::endl << std::endl;
} //end generate_friendship()


/**
 * Generates the advices that instrument the user-defined locations in
 * the model.
 */
void
generate_individually(std::ofstream& os, func_loc_t* loc, global_params* gp) {

  if(loc->func_befter == DUMMY) {
    return;
  } //end if

  os << "  // Instrumentation code for '";
  gp->func_loc_to_stream(loc, os);
  os << "'" << std::endl;
  
  std::stringstream ss;
  
  os << "  advice ";
  if (loc->func_befter == ENTRY) {
    ss << "execution(" << loc->func_identifier << "): before()";
  } //end if
  else if (loc->func_befter == EXIT) {
    ss << "execution(" << loc->func_identifier << "): after()";
  } //end else
  else if (loc->func_befter == CALL) {
    ss << "call(" << loc->func_identifier << "): before()";
  } //end else
  else {
    assert(loc->func_befter == RETURN);
    ss << "call(" << loc->func_identifier << "): after()";
  } //end else

  os << ss.str() << " {" << std::endl << std::endl;
  os << "#ifdef DEBUG_INSTRUMENTATION" << std::endl;
  os << "      std::cout << \"Join Point signature: \" << JoinPoint::signature() << std::endl;" << std::endl;
  os << "      std::cout << \"Using advice: ";

  std::string aspect_quote_escaped;
  quote_escape(ss.str(), & aspect_quote_escaped);
  os << aspect_quote_escaped << "\" << std::endl;" << std::endl;

  
  os << "      std::cout << \"For user location ";
  std::string ul_quote_escaped;
  std::stringstream ul_str;
  gp->func_loc_to_stream(loc, ul_str);
  quote_escape(ul_str.str(), & ul_quote_escaped);
  os << ul_quote_escaped << "\" << std::endl;" << std::endl;
  
  os << "#endif" << std::endl << std::endl;
  
  // Find all monitors that are using this sample point as a clock
  if (gp->all_monitors.size() > 0) {
    mpset_t::const_iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    sset_t* clocks = mp->get_clocks();
    if (clocks->find(loc->loc_name) != clocks->end()) {
      os << "    {" << std::endl;
      os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
      os << "      " << this_monitor_name << "* mon = ("
   << this_monitor_name << "*) observer->get_monitor_by_index("
   << id << ");" << std::endl;
      os << "      mon->callback_userloc_" << loc->loc_name << "();" << std::endl;
      os << "    }" << std::endl;
    } //end if
  } // end if over all monitors

  /*
  for(mpset_t::const_iterator it = gp->all_monitors.begin();
      it != gp->all_monitors.end();
      ++it) {
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    sset_t* clocks = mp->get_clocks();
    if (clocks->find(loc->loc_name) != clocks->end()) {
      os << "    {" << std::endl;
      os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
      os << "      " << this_monitor_name << "* mon = ("
	 << this_monitor_name << "*) observer->get_monitor_by_index("
	 << id << ");" << std::endl;
      os << "      mon->callback_userloc_" << loc->loc_name << "();" << std::endl;
      os << "    }" << std::endl;
    } //end if
  } // end loop over all monitors
  */

  os << "  } // advice" << std::endl << std::endl << std::endl;
} //end generate_individually()



void
instrument_return(std::ofstream& os, user_val_t* uv, global_params* gp) {

  assert (uv->param_index == 0);
  
  std::stringstream ss;
  
  os << "  // Instrumenting ";
  gp->user_val_to_stream(uv, os);
  os << std::endl;
  
  os << "  advice ";
  // Notice that we output to ss, not os.
  ss << "call(" << uv->func_identifier << "): after()";
  
  os << ss.str() << " {" << std::endl << std::endl;
  os << "#ifdef DEBUG_INSTRUMENTATION" << std::endl;
  os << "      std::cout << \"Join point signature: \" << JoinPoint::signature() << std::endl;" << std::endl;
  os << "      std::cout << \"Using advice: ";

  std::string aspect_quote_escaped;
  quote_escape(ss.str(), & aspect_quote_escaped);
	       
  os << aspect_quote_escaped << "\" << std::endl;" << std::endl;
  os << "      std::cout << \"For user value: ";
  std::string uv_str_quote_escaped;
  std::stringstream uv_str;
  gp->user_val_to_stream(uv, uv_str);
  quote_escape(uv_str.str(), & uv_str_quote_escaped);
  os << uv_str_quote_escaped << "\" << std::endl;" << std::endl;
  os << "#endif" << std::endl << std::endl;

  // Call all monitors using this value point
  if (gp->all_monitors.size() > 0) {
    mpset_t::const_iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    {" << std::endl;
    os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
    os << "      " << this_monitor_name << "* mon = ("
       << this_monitor_name << "*) observer->get_monitor_by_index("
       << id << ");" << std::endl;
    os << "      mon->value_callback_" << uv->val_name
       << "((" << uv->val_type << ") *(" << uv->val_type
       << " *) tjp->result());" << std::endl;
    os << "    }" << std::endl;
  } //end if over all monitors

  /*
  for(mpset_t::const_iterator it = gp->all_monitors.begin();
      it != gp->all_monitors.end();
      ++it) {
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    {" << std::endl;
    os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
    os << "      " << this_monitor_name << "* mon = ("
       << this_monitor_name << "*) observer->get_monitor_by_index("
       << id << ");" << std::endl;
    os << "      mon->value_callback_" << uv->val_name
       << "((" << uv->val_type << ") *(" << uv->val_type
       << " *) tjp->result());" << std::endl;
    os << "    }" << std::endl;
  } //end loop over all monitors
  */

  os << "  } // advice" << std::endl << std::endl << std::endl;
} //end instrument_return()



void
generate_individually(std::ofstream& os, user_val_t* uv, global_params* gp) {

  if (uv->param_index == 0) {
    instrument_return(os, uv, gp);
    return;
  } //end if
  
  std::stringstream ss;

  os << "  // Instrumenting ";
  gp->user_val_to_stream(uv, os);
  os << std::endl;
  
  os << "  advice ";
  // Notice that we output to ss, not os.
  ss << "execution(" << uv->func_identifier << "): before()";
  
  os << ss.str() << " {" << std::endl << std::endl;
  os << "#ifdef DEBUG_INSTRUMENTATION" << std::endl;
  os << "      std::cout << \"Join point signature: \" << JoinPoint::signature() << std::endl;" << std::endl;
  os << "      std::cout << \"Using advice: ";

  std::string aspect_quote_escaped;
  quote_escape(ss.str(), & aspect_quote_escaped);
	       
  os << aspect_quote_escaped << "\" << std::endl;" << std::endl;
    os << "      std::cout << \"For user value: ";
    
  std::string uv_str_quote_escaped;
  std::stringstream uv_str;
  gp->user_val_to_stream(uv, uv_str);
  quote_escape(uv_str.str(), & uv_str_quote_escaped);
  os << uv_str_quote_escaped << "\" << std::endl;" << std::endl;
  
  os << "#endif" << std::endl << std::endl;

  // Call all monitors using this value point
  if (gp->all_monitors.size() > 0) {
    mpset_t::const_iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    {" << std::endl;
    os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
    os << "      " << this_monitor_name << "* mon = ("
       << this_monitor_name << "*) observer->get_monitor_by_index("
       << id << ");" << std::endl;
    os << "      mon->value_callback_" << uv->val_name
       << "((" << uv->val_type << ") *(" << uv->val_type << " *)tjp->arg(" << uv->param_index - 1
       << "));" << std::endl;
    os << "    }" << std::endl;
  } //end loop over all monitors

  /*
  for(mpset_t::const_iterator it = gp->all_monitors.begin();
      it != gp->all_monitors.end();
      ++it) {
    monitor_params* mp = *it;
    
    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    
    os << "    {" << std::endl;
    os << "      mon_observer* observer = local_observer::getInstance();" << std::endl;
    os << "      " << this_monitor_name << "* mon = ("
       << this_monitor_name << "*) observer->get_monitor_by_index("
       << id << ");" << std::endl;
    os << "      mon->value_callback_" << uv->val_name
       << "((" << uv->val_type << ") *(" << uv->val_type << " *)tjp->arg(" << uv->param_index - 1
       << "));" << std::endl;
    os << "    }" << std::endl;
  } //end loop over all monitors
  */
  
  os << "  } // advice" << std::endl << std::endl << std::endl;

} //end generate_individually()


/**
 * Determines if we need to generate an Aspect file
 */
bool
aspects_needed(global_params* gp) {
  bool needed = false;
  
  std::vector<func_loc_t*>* locs = gp->get_func_locs();
  for (unsigned int i = 0; i < locs->size(); i++) {
    needed = needed || (locs->at(i)->func_befter != DUMMY);
  }
  
  needed = needed || (gp->get_user_vals()->size() > 0);

  needed = needed || (gp->user_objects.size() > 0);

  return needed;
}


/**
 * Generates the aspect file that will instrument the source code:
 */
void
generate_aspect_file(global_params* gp) {

  
  if (! aspects_needed(gp)) {
    return;
  }

  // Figure out the output directory
  std::string output_directory;
  std::string output_file = std::string(gp->get_output_file());
  std::string::size_type slash_pos = output_file.find_last_of("/");
  
  //Set output_directory
  if (slash_pos == std::string::npos) {
    // No slash, store in the current directory
    output_directory += "./";
  } //end if
  else {
    output_directory = output_file.substr(0, slash_pos + 1);
  } //end else

  std::string aspect_filename;
  //Ensure the file name is unique by adding in a random  number, if necessary
  get_unique_file_name(& aspect_filename, & output_directory, "aspect_definitions", "ah"); //utils.cc

  std::cout << "Outputting aspect to " << aspect_filename << std::endl;
  std::ofstream& os = *open_for_output(aspect_filename.c_str()); //utils.cc:open_for_output

  std::string capitalized;       //the #ifdef version of the filename
  std::string naked_header_file; //the plain file name, stripped of the path
  //get the file name capitalized, without path, with s/./_/g so suitable for a #ifdef argument 
  get_capitalized_name(aspect_filename.c_str(), & capitalized); //utils.cc
  //return the file name without the leading path:
  get_naked_file_name(gp->get_header_output_file(), & naked_header_file); //utils.cc
  
  //Print out the definitions to the aspect file
  os << "#ifndef " << capitalized << std::endl;              //#ifndef FILENAME
  os << "#define " << capitalized << std::endl << std::endl; //#define FILENAME
  os << "#include <systemc>" << std::endl;
  os << "#include \"" << naked_header_file << "\"" << std::endl << std::endl;
  os << "#include <iostream>" << std::endl << std::endl;
  os << "aspect Automatic" << " {" << std::endl;

  //Print out the friend monitor objects into the aspect file
  generate_friendship(os, gp); //aspect.cc
  
  std::vector<func_loc_t*>* locs = gp->get_func_locs();
  for (unsigned int i = 0; i < locs->size(); i++) {
    generate_individually(os, locs->at(i), gp);
  } //end for

  std::vector<user_val_t*>* uvs = gp->get_user_vals();
  for (unsigned int i = 0; i < uvs->size(); i++) {
    generate_individually(os, uvs->at(i), gp);
  } //end for

  //Print the end of the aspect file definition
  os << "}; // Automatic"  << std::endl << std::endl << std::endl;
  os << std::endl << "#endif" << std::endl;
  close_file(&os);
} //end generate_aspect_file
