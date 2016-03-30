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


#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <algorithm>
#include <string>

#include <sstream>
#include <iostream>
#include <fstream>

// For file status checking
#include <sys/stat.h>
#include "types.h"


#ifdef TIMING

void start_timer();
void display_timer(const char* message);
void start_final_timer();
void stop_final_timer();
void start_ea_timer();
void display_ea_timer();

#endif

bool is_event_clock(std::string* event_expr);
std::string* get_event_clock_extension();
std::string* get_event_from_clock(std::string* clock);
sset_t* get_event_clocks_subset(sset_t* clocks);
std::string* clock_event_to_monitor_variable(std::string* clock);

void declare_forward(std::string* user_object, std::ostream& os);

void copyright_generation(std::ostream& os);

// Add a "//" at the beginning of each line
void comment_out_string(std::string* input);

void remove_str(const std::string source, std::string flags, std::ostream& os);
void remove_str(const std::string* input, const char* flags, std::string* output);
void quote_escape(std::string source, std::string* result);
bool prefix2inorder(std::ifstream& in, std::string* dest);
int str2int(const std::string& str);

void copy_without_surrounding_whitespace(const std::string* source,
					 std::string* destination);

std::ofstream* open_for_output(const char* file_name);

void close_file(std::ofstream* file_handle);

void get_unique_file_name(std::string* container,
			  std::string* directory,
			  const char* prefix = "temp_file",
			  const char* extension = "tmp");

void get_unique_file_name_in_same_directory(std::string* neighbor_file,
					    const char* prefix,
					    const char* extension,
					    std::string* result);

void get_naked_file_name(const char* full_name, std::string* container);
void get_capitalized_name(const char* file_name, std::string* container);

#endif
