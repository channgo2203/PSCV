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

#include "observer.h"


/*
 *
 */
void generate_local_observer(std::ostream& os, std::ostream& header_os, 
	global_params* gp) {

  header_os << std::endl << std::endl;
  header_os << "/*" << std::endl;
  header_os << " * This object is derived from mon_observer defined in the SystemC" << std::endl;
  header_os << " * modified kernel. Thus, we can pass it as an argument to the classes" << std::endl;
  header_os << " * implementing the monitors." << std::endl;
  header_os << " */" << std::endl;  
  header_os << "class local_observer : public sc_core::mon_observer {" << std::endl << std::endl;
  header_os << " public:" << std::endl << std::endl;
  header_os << "  // Constructor" << std::endl;
  header_os << "  local_observer(unsigned int verbosity";

  // Determine what objects need to be passed to the observer
  std::stringstream observer_objects;
  std::stringstream observer_object_firsts;
  for(ssmap_t::const_iterator it = gp->all_objects.begin();
      it != gp->all_objects.end();
      ++it) {

	  observer_objects << ", " << it->second << " " << it->first;
	  observer_object_firsts << ", " << it->first;
  }

  header_os << observer_objects.str() << ");"
	        << std::endl << std::endl;

  // Now the implementation
  os << "// static instance variable" << std::endl;
  os << "local_observer* local_observer::m_instance;"
     << std::endl << std::endl;

  os << std::endl << std::endl;
  os << "/*" << std::endl;
  os << " * Constructor" << std::endl;
  os << " */" << std::endl;
  os << "local_observer::local_observer(unsigned int verbosity";
  os << observer_objects.str() << ") : sc_core::mon_observer(verbosity) {"
     << std::endl << std::endl;

  // Instantiate the monitors
  if (gp->all_monitors.size() > 0) {
    mpset_t::iterator it = gp->all_monitors.begin();
    monitor_params* mp = *it;
      std::string id;
      mp->get_id_str(&id);
      std::string this_monitor_name = gp->get_mon_name() + id;

      std::string this_monitor_inst = this_monitor_name + "_inst";

      os << "  " << this_monitor_name << "* " 
         << this_monitor_inst
         << " = new " << this_monitor_name << "(this";

      // Determine if this monitor needs any objects passed
      for(ssmap_t::const_iterator sit = gp->get_vartypes()->begin();
         sit != gp->get_vartypes()->end();
         ++sit)
    {
         os << ", " << sit->first;
    }
      os << ");" << std::endl;
      
      os << "  mons.push_back(" << this_monitor_inst 
         << ");" << std::endl << std::endl;
  }
  // Constructor ends here.
  os << "}" << std::endl << std::endl;

  /*
  for(mpset_t::iterator it = gp->all_monitors.begin();
      it != gp->all_monitors.end();
      ++it)
  {
      monitor_params* mp = *it;
      std::string id;
      mp->get_id_str(&id);
      std::string this_monitor_name = gp->get_mon_name() + id;

      std::string this_monitor_inst = this_monitor_name + "_inst";

      os << "  " << this_monitor_name << "* " 
         << this_monitor_inst
	       << " = new " << this_monitor_name << "(this";

      // Determine if this monitor needs any objects passed
      for(ssmap_t::const_iterator sit = gp->get_vartypes()->begin();
	       sit != gp->get_vartypes()->end();
	       ++sit)
	  {
	       os << ", " << sit->first;
	  }
      os << ");" << std::endl;
      
      os << "  mons.push_back(" << this_monitor_inst 
         << ");" << std::endl << std::endl;
  }
  // Constructor ends here.
  os << "}" << std::endl << std::endl;
  */

  // Destructor
  header_os << "  ~local_observer();"
		    << std::endl << std::endl;

  // Now the implementation
  os << "/*" << std::endl;
  os << " * Destructor" << std::endl;
  os << " */" << std::endl;
  os << "local_observer::~local_observer() {"
     << std::endl << std::endl;
  os << "  if (m_instance)" << std::endl 
     << "    delete m_instance;" << std::endl 
     << std::endl; 
  os << "}" << std::endl << std::endl;

  // Get instance
  header_os << "  static local_observer* getInstance();"
		    << std::endl << std::endl;

  // Create instance
  header_os << "  static local_observer* createInstance(unsigned int verbosity";
  header_os << observer_objects.str() << ");"
		    << std::endl << std::endl;
		    
  // Instance variable
  header_os << " private:" << std::endl << std::endl;
  header_os << "  static local_observer* m_instance;"
		    << std::endl << std::endl;

  // End of header file
  header_os << "};" << std::endl;
  
  // Now the implementation
  os << "/*" << std::endl;
  os << " * Create instance" << std::endl;
  os << " */" << std::endl;
  os << "local_observer* local_observer::createInstance(unsigned int verbosity";
  os << observer_objects.str() << ") {"
     << std::endl << std::endl;

  os << "  if (m_instance == NULL) {" << std::endl;
  os << "    m_instance = new local_observer(verbosity";
  os << observer_object_firsts.str() << ");"
	 << "  }" << std::endl << std::endl;

  os << "  return m_instance;" << std::endl << std::endl;
  // End of create instance
  os << "}" << std::endl << std::endl;

  // Get instance
  os << "/*" << std::endl;
  os << " * Get instance" << std::endl;
  os << " */" << std::endl;
  os << "local_observer* local_observer::getInstance() {"
     << std::endl << std::endl;
  os << "  return m_instance;" << std::endl << std::endl;
  os << "}" << std::endl;

}
