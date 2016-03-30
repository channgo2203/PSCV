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


#ifndef TYPES_H
#define TYPES_H

#include <set>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <stdint.h>

// Forward declaration
class monitor_params;

typedef enum {
  FRONT_NONDET, 
  FRONT_DET_SWITCH,
  FRONT_DET_IFELSE,
  FRONT_DET_FILE_TABLE,
  FRONT_DET_MEMORY_TABLE,
  BACK_NONDET,
  BACK_ASS_ALPHA,
} encoding_t;

typedef enum {
  BEST_OF_ALL,
  FILE_TABLE,
  MEMORY_TABLE,
} configuration_t;

typedef enum {
  PARTIAL_REDUCTION,
  FULL_REDUCTION,
  NO_REDUCTION
} alpha_reduction_t;

typedef enum {
  ASSIGNMENTS,
  SYMBOLIC
} alphabetization_t;

typedef enum {
  BEFORE,
  AFTER
} befter_t;

typedef enum {
  ENTRY,
  EXIT,
  CALL,
  RETURN,
  DUMMY
} func_befter_t;

typedef struct FunctionLocation {
  std::string loc_name;
  std::string func_identifier;
  func_befter_t func_befter;
} func_loc_t;

// plocation - location of particular statement
typedef struct PatternLocation {
  std::string loc_name;
  std::string pattern;
} plocation_t;

// return or argument value of a function
typedef struct UserValue {
  std::string val_type;
  std::string val_name;
  std::string func_identifier;
  int param_index;
} user_val_t;

typedef std::map<std::string, std::string> ssmap_t;
typedef std::set<std::string> sset_t;
typedef std::set<monitor_params*> mpset_t;
typedef std::list<std::string> slist_t;
typedef std::vector<std::string> svector_t;
typedef std::vector< std::map<std::string, bool>* > v_sbmap_t;

#endif
