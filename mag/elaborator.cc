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

*/


#include <iostream>


int nested;



void print_array(bool* ar) {
  
  for (int i = 0; i < nested; i++) {
    std::cout << ( (ar[i] ) ? " " : "!") ;
    std::cout << "\"my_adder->_a == " << i * 100 << "\"";
    if (i != nested - 1) {
      std::cout << " && ";
    }
  }
}


void cond_print(bool* ar) {
  int count = 0;

  for (int i = 0; i < nested; i++) {
    if (ar[i]) {
      count ++;
    }
  }
  
  if (count > 1) {
    std::cout << "&& G(!(";
    print_array(ar);
    std::cout << ")) ";// << std::endl;
  }
}



int main(int args, char** argv) {
  
  if (args < 2) {
    std::cout << argv[0] << " d" << std::endl;
    std::cout << "Where d is the number of nested Untils " << std::endl;
    exit(0);
  }

  nested = atoi(argv[1]);
  bool ba[nested];
  
  for (int i=0; i< 1<< nested; i++) {
    
    for (int j=nested-1; j>=0; j--) {
      int bit = ((i >> j) & 1);
      ba[nested - j - 1] = bit;
    }

    cond_print(ba);
  }
  
  std::cout << std::endl;
  exit (0);
}
