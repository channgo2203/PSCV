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


#ifndef PLASMA_GEN_H
#define PLASMA_GEN_H

#include <stdio.h>

#include "global_params.h"
#include "monitor_params.h"
#include "utils.h"
#include "types.h"


class plasma_gen {
 
 public:
 	// Constructor
 	plasma_gen(global_params* g);

 	// Virtual destructor
 	virtual ~plasma_gen() {};

	// plasma project file generation
	void generate_plasma_file();

 protected:
 	global_params* gp;
};


#endif