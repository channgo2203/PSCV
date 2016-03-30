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


#ifndef MONITOR_PARAMS
#define MONITOR_PARAMS

#include <string>
#include <iostream>
#include <cassert>
#include <sstream>

#include "types.h"


class monitor_params {

 public:

  // Constructor
  monitor_params();


  void add_formula(std::string parsed_formula);
  void add_formula(char* formula) { add_formula(std::string(formula)); }

  void add_timeresolution(std::string parsed_formula);
  void add_timeresolution(char* formula) { add_timeresolution(std::string(formula)); }
  void set_timeresolution(const sset_t& t) { clocks = t; }
  
  void get_formula(std::string* container) { container->assign(ltl); }
  void reset_formula(std::string* new_formula) { ltl = *new_formula; }
  void get_formula_name(std::string* container) { container->assign(formula_name); }
  void reset_formula_name(std::string* new_name) { formula_name = *new_name; }

  void set_formula(std::string parsed_formula);
  void set_formula(char* formula) { set_formula(std::string(formula)); }


  /*
   * Return all clocks
   */
  sset_t* get_clocks() { return &clocks; }
  

  /*
   * Stores another pair of (variable, type)
   */
  void add_vartype(std::string var, std::string type) { vartypes[var] = type; }
  ssmap_t* get_vartypes() { return &vartypes; }


  /*
   * Stores another pair of (label, attribute)
   */
  void add_attributelabel(std::string lbl, std::string attr) {attributelabels[lbl] = attr;}
  ssmap_t* get_attributelabels() {return &attributelabels;}


  /*
   * Stores another pair of (label, eventclk)
   */
  void add_eventclocklabel(std::string lbl, std::string eventclk) {eventclocklabels[lbl] = eventclk;}
  ssmap_t* get_eventclocklabels() {return &eventclocklabels;}


  void to_stream(std::ostream& os) const;
  void to_string(std::string*);

  void get_id_str(std::string*);
  void get_f_quote_escaped(std::string* cont) { cont->assign(f_quote_escaped); }

  
private:
  std::string ltl; // PBLTL formula before "@"
  std::string formula_name; // formula name behind "@"
  std::string formula; // formula = pbltl@name
  std::string f_quote_escaped; // a quote-escaped version of the formula- replace \" by \\\"
  
  sset_t clocks; // Set of all clock expressions. If no clock specified by user, it contains {"DEFAULT CLOCK"}
  std::string clk_formula; // clk_formula = clk1||clk2||...||clkn
  std::string clk_quote_escaped;

  ssmap_t vartypes; // Ex: type adder* addy - vartype= {(addy, adder*)}
  ssmap_t attributelabels;
  ssmap_t eventclocklabels;

  int id; // ids to num_monitors
  static int num_monitors; // Total Number of monitors = Total number of PBLTL specifications

};


#endif
