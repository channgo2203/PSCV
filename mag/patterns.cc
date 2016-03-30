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

#include "patterns.h"

bool
patterns_needed(global_params* gp) {

  std::vector<plocation_t*>* locs = gp->get_plocations();
  return locs->size() > 0;
}


void
generate_individually(std::ofstream& os, plocation_t* loc, global_params* gp) {

  os << "  # Instrumentation code for "
     << loc->loc_name << " = "
     << "-->" << loc->pattern << "<--"
     << std::endl;

  std::stringstream sed_command;
  sed_command << "  sed -i \"s/\\(^" << loc->pattern << "$\\)/";

  // Find all monitors that are using this sample point as a clock
  if (gp->all_monitors.size() > 0) {
    mpset_t::const_iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;

    std::string id;
    mp->get_id_str(&id);
    std::string this_monitor_name = gp->get_mon_name() + id;
    sset_t* clocks = mp->get_clocks();
    if (clocks->find(loc->loc_name) != clocks->end()) {
      os << sed_command.str()
   << "mon_observer* observer = local_observer::getInstance();\\n"
   << this_monitor_name << "* mon = ("
   << this_monitor_name << "*) observer->get_monitor_by_index("
   << id << ");\\n"
   << "mon->callback_userloc_" << loc->loc_name << "();\\n"
   << "\\1\\n/g\" $i" << std::endl;
      os << "  sed -i '1i#include \"" << gp->get_header_output_file() << "\"' $i" << std::endl;
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
      os << sed_command.str()
	 << "mon_observer* observer = local_observer::getInstance();\\n"
	 << this_monitor_name << "* mon = ("
	 << this_monitor_name << "*) observer->get_monitor_by_index("
	 << id << ");\\n"
	 << "mon->callback_userloc_" << loc->loc_name << "();\\n"
	 << "\\1\\n/g\" $i" << std::endl;
      os << "  sed -i '1i#include \"" << gp->get_header_output_file() << "\"' $i" << std::endl;
    } //end if
  } // end loop over all monitors
  */
}

void
generate_patterns_file(global_params* gp) {

  if (! patterns_needed(gp)) {
    return;
  } //end if

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

  std::string pattern_filename;
  get_unique_file_name(& pattern_filename, & output_directory, "instrument", "sh");

  std::cout << "Outputting patterns to " << pattern_filename << std::endl;
  std::ofstream& os = *open_for_output(pattern_filename.c_str());

  os << "#!/bin/bash" << std::endl;
  os << "for i in `find . -name \"*.cc\" -o -name \"*.cpp\"`" << std::endl;
  os << "do" << std::endl;
  os << "  echo Processing $i" << std::endl << std::endl;
  std::vector<plocation_t*>* locs = gp->get_plocations();
  for (unsigned int i = 0; i < locs->size(); i++) {
    generate_individually(os, locs->at(i), gp);
    os << std::endl;
  }

  os << "done" << std::endl;

  close_file(&os);
}




