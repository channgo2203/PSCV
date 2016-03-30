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


#include "monitor_params.h"


int monitor_params::num_monitors = 0;


/*
 * Constructor
 */
monitor_params::monitor_params() {
  id = num_monitors++;
}


void monitor_params::get_id_str(std::string* container) {
  std::stringstream ss;
  ss << id;
  container->assign(ss.str());
}


/*
 * Cleans up the parsed_formula from which the monitor will be
 * generated. Expected format for the parsed_formula is LTL @ name
 */
void monitor_params::add_formula(std::string parsed_formula) {

  // Remove leading white spaces
  char const* delims = " \t\f\v\r\n";
  std::string::size_type notwhite = parsed_formula.find_first_not_of(delims);
  parsed_formula.erase(0, notwhite);

  // Find the @
  std::string::size_type separator = parsed_formula.find('@');
  // No name in the parsed formula
  if (separator == std::string::npos) {
    // the ltl
    ltl = parsed_formula;
    // the name
    std::string id;
    get_id_str(&id);
    formula_name = std::string("req") + id;
  }
  // There is a name for the current formula
  else {
    ltl = parsed_formula.substr(0, separator);
    std::string name_str = parsed_formula.substr(separator + 1);

    // trim leading garbage
    char const* name_delims = " \t\f\v\n\r";

    std::string::size_type notwhite = name_str.find_first_not_of(name_delims);
    
    // name string is all white space or garbage
    if (notwhite == std::string::npos) {
      std::string id;
      get_id_str(&id);
      formula_name = std::string("req") + id;
    }
    // a name is given
    else {
      name_str.erase(0, notwhite);
    
      // Trim trailing garbage
      notwhite = name_str.find_last_not_of(name_delims);

      // There must be nonwhite characters, otherwise we would have
      // taken the other branch of the conditional
      assert(notwhite != std::string::npos);
      
      name_str.erase(notwhite + 1);
      // clean name
      formula_name = name_str;
    }
  }

  // Return clean formula
  formula = ltl;
  formula.append(" @ ");
  formula.append(formula_name);

  // Finally, generate a quote-escaped version of the formula
  f_quote_escaped = formula;
  unsigned long int j = f_quote_escaped.find("\"");
  while (j < std::string::npos) {
    f_quote_escaped.replace(j, 1, "\\\"");
    j = f_quote_escaped.find("\"", j+2);
  }
}


/*
 * Cleans up the parsed_formula from which the monitor will be
 * generated. Expected format for the parsed_formula is CLK||CLK||CLK ...
 */
void monitor_params::add_timeresolution(std::string parsed_formula) {
  // Remove leading whitespace
  char const* clock_delims = " \t\f\v\n\r|";
  std::string::size_type notwhite = parsed_formula.find_first_not_of(clock_delims);

  // Empty clocks
  if (notwhite == std::string::npos) {
    clocks.insert(std::string("DEFAULT_CLOCK"));
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
        clocks.insert(parsed_formula.substr(pos));
        // Reset the loop's flag
        pos = white;
      }
      else {
        clocks.insert(parsed_formula.substr(pos, white - pos));
        pos = parsed_formula.find_first_not_of(clock_delims, white);
      }
    }    
  }

  // Return the clean time resolution
  clk_formula = std::string("");

  for (sset_t::const_iterator it = clocks.begin(); it != clocks.end(); it++) {
    if (it != clocks.begin()) {
      clk_formula.append(" || ");
    }
    clk_formula.append(*it);
  }

  // Finally, generate a quote-escapte version of the time resolution
  clk_quote_escaped = clk_formula;
  unsigned long int j = clk_quote_escaped.find("\"");
  while (j < std::string::npos) {
    clk_quote_escaped.replace(j, 1, "\\\"");
    j = clk_quote_escaped.find("\"", j+2);
  }

}


/*
 * Cleans up the parsed_formula from which the monitor will be
 * generated. Expected format for the parsed_formula is LTL @ CLOCK ||
 * CLOCK || CLOCK ...
 */
void monitor_params::set_formula(std::string parsed_formula) {
  
  // Remove leading whitespace
  char const* delims = " \t\f\v\r\n";
  std::string::size_type notwhite = parsed_formula.find_first_not_of(delims);
  parsed_formula.erase(0,notwhite);

  std::string::size_type separator = parsed_formula.find('@');
  
  if (separator == std::string::npos) {
    // No clock in the parsed_formula
    clocks.insert(std::string("DEFAULT_CLOCK"));
    ltl = parsed_formula;
  }
  
  else {
    ltl = parsed_formula.substr(0, separator);
    std::string clock_str = parsed_formula.substr(separator + 1);

    // trim leading garbage
    char const* clock_delims = " \t\f\v\n\r|";

    std::string::size_type notwhite = clock_str.find_first_not_of(clock_delims);
    
    if (notwhite == std::string::npos) {
      // Clock string is all white space or garbage
      clocks.insert(std::string("DEFAULT_CLOCK"));
    }

    else {
      clock_str.erase(0, notwhite);
    
      // Trim trailing garbage
      notwhite = clock_str.find_last_not_of(clock_delims);

      // There must be nonwhite characters, otherwise we would have
      // taken the other branch of the conditional
      assert(notwhite != std::string::npos);
      clock_str.erase(notwhite + 1);
      
      unsigned long int pos = 0;
      while (pos != std::string::npos) {
	       std::string::size_type white = clock_str.find_first_of(clock_delims, pos);
      
	       if (white == std::string::npos) {
	         // There are no white space characters
	         clocks.insert(clock_str.substr(pos));
	         // Reset the flag for the loop.
	         pos = white;
	       }
	       else {
	         clocks.insert(clock_str.substr(pos, white-pos));
	         pos = clock_str.find_first_not_of(clock_delims, white);
	       }
      }
    }
  }
  
  
  // Here we generate a clean version of the full formula, i.e. ltl @
  // clock, for display back to the user
  formula = ltl;
  formula.append(" @ ");
  for (sset_t::const_iterator it = clocks.begin();
       it != clocks.end();
       it++)
    {
      if (it != clocks.begin()) {
	       formula.append(" || ");
      }
      formula.append( *it);
    }

  // Finally, generate a quote-escaped version of the formula
  f_quote_escaped = formula;
  unsigned long int j = f_quote_escaped.find("\"");
  while (j < std::string::npos) {
    f_quote_escaped.replace(j, 1, "\\\"");
    j = f_quote_escaped.find("\"", j+2);
  }
} // set formula


/*
 * Print the formula and time resolution
 */
void monitor_params::to_stream(std::ostream &outstr) const {
  
  outstr << "\t PBLTL forumula: " << ltl << std::endl;
  outstr << "\t clock(s): " << std::endl; 
  
  for (sset_t::iterator it = clocks.begin(); 
      it != clocks.end(); 
      ++it ) {
      outstr << "\t\t\t" << *it << std::endl;
  }
}

void monitor_params::to_string(std::string* container) {
  std::stringstream ss( std::stringstream::out );
  to_stream(ss);
  container->assign(ss.str());
}
