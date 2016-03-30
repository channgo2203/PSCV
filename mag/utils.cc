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

#include <ctype.h>
#include "utils.h"


/*
 * Parses an integer from the string @str
 */
int str2int(const std::string& str) {
  std::stringstream ss(str);
  int num;
  if ((ss >> num).fail()) {
    std::cerr << __FILE__ << ": " << __func__ << "():" << std::endl;
    std::cerr << "Failed to parse an integer from the string " << str << std::endl;
    std::cerr << "The error is probably due to the caller of this function." << std::endl;
    exit(1);      
  }

  return num;
}


/*
 * Comments-out the string @input by adding a "//" at the beginning of
 * each line. After this modification, the string can be inserted into
 * any C/C++ generated file.
 */
void comment_out_string(std::string* input) {
  
  // Comment out the first line
  input->replace ( 0, 0, "//" );

  std::string::size_type pos = input->find ("\n", 0); 
  while (  pos != std::string::npos ) {
    input->replace ( pos, 1, "\n//" );
    pos = input->find ("\n", pos + 3);
  } //end while
} //end comment_out_string()


/*
 * Removes any characters from @flags from @source. Rest is output to @os
 */
void remove_str(std::string source, std::string flags, std::ostream& os) {
  
  unsigned int left = 0;

  while (left < source.size()) {    
    unsigned long int right = source.find_first_of(flags, left);
    if (right == std::string::npos) {
      // No more instances of the characters in @flags
      os << source.substr(left); // Get the rest of the string
      left = source.size(); // Break out of the while()
    } //end if
    else {
      os << source.substr(left, right-left);
      left = right + 1;
    } //end else
  } //end while
} //end remove_str()
  

/*
 * Removes all characters mentioned in @flags from @input. The result
 * is returned inside @output.
 */
void remove_str(const std::string* input, const char* flags, std::string* output) {
  std::stringstream ss;
  remove_str(*input, std::string(flags), ss);
  output->assign(ss.str());
} //end remove_str()


/*
 * Converts an LTL formula in prefix normal form into an in-order form.
 */
bool prefix2inorder(std::ifstream& in, std::string* dest) {
  
  std::string space = std::string(" ");
  std::string lparen = std::string("(");
  std::string rparen = std::string(")");
  std::string left;
  std::string right;
  std::string op;
  char ch;
  
  // Skip over the initial white space
  do {
    ch = in.get();
  }while(isspace(ch));

  switch (ch) {
  case 'U':
  case 'u':
  case 'V':
  case 'v':
  case 'i':
  case 'e':
  case '&':
  case '|':
    if (prefix2inorder(in, &left)) {

      if (prefix2inorder(in, &right)) {
	switch (ch) {
	case 'U':
	case 'u':
	case 'V':
	case 'v':
	  op = std::string(1, ch);
	  break;

	case '&':
	case '|':
	  // Repeat the character twice
	  op = std::string(2, ch);
	  break;
	  
	case 'i':
	  op = std::string("->");
	  break;
	  
	case 'e':
	  op = std::string("<->");
	  break;
	  
	} //end switch
	
	(*dest) = lparen + left + space + op + space + right + rparen;
	return true;
      } //end if
    } //end if
    return false;

  case '!':
  case 'X':
  case 'G':    
  case 'F':
    if (prefix2inorder(in, &right)) {
      (*dest) = lparen + std::string(1, ch) + space + right + rparen;
#ifdef DEBUG_UTILS
      std::cout << __FILE__ << ": " << __func__ << " Parsed " << *dest << std::endl;
#endif
      return true;
    } //end if
    return false;

  case 't':
    (*dest) = std::string("1");
#ifdef DEBUG_UTILS
	std::cout << __FILE__ << ": " << __func__ << " Parsed " << *dest << std::endl;
#endif
    return true;
    
  case 'f':
    (*dest) = std::string("0");
#ifdef DEBUG_UTILS
	std::cout << __FILE__ << ": " << __func__ << " Parsed " << *dest << std::endl;
#endif
    return true;

  case 'p':    
    {	  
      while (!isspace(ch) && in.good()) {
	(*dest) += std::string(1, ch);
	ch = in.get();
      } //end while
    } //Why do we need this set of {}s?
#ifdef DEBUG_UTILS
	std::cout << __FILE__ << ": " << __func__ << " Parsed " << *dest << std::endl;
#endif    
    return true;
    
  case EOF:
    std::cerr << __FILE__ << ": " << __func__ << ": Parse error. Unexpected end of file." << std::endl;;
    return false;
    
  default:
    std::cerr << __FILE__ << ": " << __func__ << ": Parse error. Illegal character \""
	      << ch << "\"" << std::endl;;
    return false;
  
  } //end switch
} //end prefix2inorder()


/*
 * Removes leading and trailing whitespace from the string @source,
 * and copies the remaining to the string @destination.
 */
void copy_without_surrounding_whitespace(const std::string* source,
					 std::string* destination) {
  // Remove leading whitespace
  std::string source_copy = *source;
  char const* delims = " \t\f\v\r\n";
  std::string::size_type notwhite = source_copy.find_first_not_of(delims);
  source_copy.erase(0,notwhite);

  // Remove trailing whitespace
  notwhite = source_copy.find_last_not_of(delims);
  source_copy.erase(notwhite + 1);
  destination->assign(source_copy);
}


/*
 * Opens a file for output, and checks for errors before returning a
 * pointer to the file handle.
 */
std::ofstream* open_for_output(const char* file_name) {
  std::ofstream* out_strm = new std::ofstream(file_name, std::ios::out);
  if (! out_strm->good()) {
    std::cerr << "Unable to open \"" << file_name << "\" for output." << std::endl;
    exit (1);
  } //end if

  return out_strm;
}

/*
 * Closes a file previously opened with open_for_output()
 */
void close_file(std::ofstream* file_handle) {
  file_handle->close();
  delete file_handle;
} //end close_file()


/*
 * Generates a unique file name and returns it in the string pointed
 * to by @container. It uses the directory of the file @neighbor_file
 * and uses @prefix and @extension to generate the name. If necessary,
 * a random number is inserted after the prefix and before the dot and
 * the extension.
 */
void get_unique_file_name_in_same_directory(std::string* neighbor_file,
					    const char* prefix,
					    const char* extension,
					    std::string* result) {

  
  std::string::size_type slash_location = neighbor_file->find_last_of("/");
  std::string directory;

  if (slash_location == std::string::npos) {
    // There is no slash in the neighbor file name. We are outputting in the
    // current directory.
    directory = "./";
  } //end if
  
  else {
    directory = neighbor_file->substr(0, slash_location);
  } //end else

  get_unique_file_name(result, &directory, prefix, extension);  
}


/*
 * Returns a unique filename in the specified directory. 
 * The filename is made unique by including a random number.
 * It creates an empty file to prevent multiple calls to this function 
 * from returning the same file name.
 */
void get_unique_file_name(std::string* container,
			  std::string* directory,
			  const char* prefix,
			  const char* extension) {
  
  // Make sure that the directory name ends with a "/"
  if (directory->find_last_of('/') != (directory->size() - 1)) {
    directory->append("/");
  } //end if
  
  std::string base_name = *directory + prefix;
  std::string try_name = base_name + "." + extension;
  
  struct stat file_info;
  int status;

  while (true) {
    status = stat(try_name.c_str(), &file_info);
    
    if (status == 0) {
      // A file with that name exists. Lets add a random integer at
      // the end of the file name, before the extension.
      std::stringstream ss;
      ss << rand();
      try_name = base_name + ss.str() + "." + extension;
    } //end if
    else {
      // Unique file found!
      container->assign(try_name);

      // Open and close to reserve the filename
      std::ofstream& handle = *open_for_output(try_name.c_str());
      close_file(& handle);
      return;
    } //end else
  } //end while
} //end get_unique_file_name()


/*
 * Removes the path from the name of the file and returns the naked
 * file name.
 */
void get_naked_file_name(const char* full_name, std::string* container) {

  std::string full_file_name = std::string(full_name);
  std::string naked_name;
  
  std::string::size_type slash_pos = full_file_name.find_last_of("/");
  if (slash_pos == std::string::npos) {
    // No slashes, so no path to strip
    naked_name = full_file_name;
  } //end if
  else {
    naked_name = full_file_name.substr(slash_pos + 1);
  } //end else
  container->assign(naked_name);
} //end get_naked_file_name()


/*
 * Removes the path from the name of the file, capitalizes all
 * letters, and replaces dots with underscores. The returned name is
 * suitable to be used as an argument for #ifdef in the header files.
 */
void get_capitalized_name(const char* file_name, std::string* container) {

  std::string cn;
  //Get the file name without the leading path
  get_naked_file_name(file_name, & cn);
  
  // Capitalize the string
  std::transform(cn.begin(), cn.end(), cn.begin(), toupper);
  
  // And convert the dots to underscores
  for (std::string::iterator it = cn.begin();
    it != cn.end();
    it++) {
    if (*it == '.') {
      *it = '_';
    } //end if
  } //end for
  container->assign(cn);
} //end get_capitalized_name()


/*
 * Quote-escape all quotes in the string
 */
void quote_escape(std::string source, std::string* result) {
  
  std::string source_copy = source;
  unsigned long int j = source_copy.find("\"");
  while (j < std::string::npos) {
    source_copy.replace(j, 1, "\\\"");
    j = source_copy.find("\"", j+2);
  } //end while
  result->assign(source_copy);

} //end quote_escape()

#ifdef TIMING
#include <sys/times.h>
#include <stdint.h>

clock_t st_time;
clock_t final_start_time;
clock_t ea_start_time;

void start_final_timer(){
        struct tms st_cpu;
        final_start_time = times(&st_cpu);
}

void stop_final_timer(){
        struct tms en_cpu;
        clock_t final_end_time = times(&en_cpu);
        printf("Total MAG time, %jd\n", (intmax_t)(final_end_time-final_start_time));
}

void start_ea_timer(){
        struct tms st_cpu;
        ea_start_time = times(&st_cpu);
}

void display_ea_timer(){
        struct tms en_cpu;
        clock_t ea_end_time = times(&en_cpu);
        printf("Edge Abbreviation, %jd\n", (intmax_t)(ea_end_time-ea_start_time));
}

void start_timer() {
        struct tms st_cpu;
        st_time = times(&st_cpu);
} //end start_timer


void display_timer(const char* message) {
        struct tms en_cpu;
        clock_t en_time = times(&en_cpu);
        printf("%s, %jd\n", message, (intmax_t)(en_time-st_time));
} //end display_and_reset_timer

#endif


// Event utilities
std::string* get_event_clock_extension() {
  return new std::string(".notified");
}


std::string* get_event_from_clock(std::string* clock) {
  std::string* notified = get_event_clock_extension();

  int last_index = clock->length() - notified->length();
  std::string event_name = clock->substr(0, last_index);

  delete notified;

  return new std::string(event_name);
}


/*
 * Check an event clock
 */
bool is_event_clock(std::string* clock) {
  std::string* notified = get_event_clock_extension();
  unsigned int search_length = notified->length();

  if (clock->length() <= search_length) {
    return false;
  }

  int search_start = clock->length() - search_length;
  bool ret = clock->substr(search_start, search_length) == *notified;
  delete notified;

  return ret;
}


sset_t* get_event_clocks_subset(sset_t* clocks) {

  sset_t* event_clocks = new sset_t();

  for (sset_t::iterator it = clocks->begin(); it != clocks->end(); ++it) {
    std::string clock = *it;

    if (is_event_clock(&clock)) {
      event_clocks->insert(clock);
    }
  }

  return event_clocks;
}


std::string* clock_event_to_monitor_variable(std::string* clock) {

  unsigned int length = clock->size();
  char buffer[length];
  unsigned int curr_position = 0;

  for(unsigned int i = 0; i < length; i++) {
    char c = clock->at(i);
    if (std::isalnum(c)) {
      buffer[curr_position] = c;
      curr_position++;
    }
  }

  buffer[curr_position] = 0;

  return new std::string(buffer);

}


void declare_forward(std::string* user_object, std::ostream& os) {

  size_t scope = user_object->find("::");
  
  if (scope != std::string::npos) {
    std::string first = user_object->substr(0, scope);
    std::string rest = user_object->substr(scope + 2);
    os << "namespace " << first << " {" << std::endl;

    declare_forward(&rest, os);

    os << "}" << std::endl;
  }

  else {
    os << "class " << *user_object << ";" << std::endl;
  }

}


void copyright_generation(std::ostream& os) {
  
  os << "/*" << std::endl;
  os << " * This code is automatically generated by MAG" << std::endl;
  os << " * Copyright (C) 2014 Chan Ngo, INRIA" << std::endl;
  os << " * chan.ngo@inria.fr" << std::endl;
  os << " */" << std::endl;
  os << std::endl << std::endl;

}
