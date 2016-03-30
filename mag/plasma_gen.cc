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


#include "plasma_gen.h"
#include "tinyxml2.h"


/*
 * constructor
 */
plasma_gen::plasma_gen(global_params* g) : gp(g) {

	#ifdef DEBUG_PLASMA_GEN
	std::cout << "plasma_gen constructor" << std::endl;
	#endif

}

 	
/*
 * plasma project file generation
 */
void plasma_gen::generate_plasma_file() {

	// create an output file
	FILE* xmlfile = fopen(gp->get_projectfile(), "w");

	// check the openning error
	if (xmlfile == NULL) {
		std::cerr << "Unable to open " 
				  << gp->get_projectfile() << " for output." << std::endl;
    	
    	exit (1);
	}

	// create xml file with tinyxml
	std::string requirementname = std::string("");
	std::string requirementformula = std::string("");


	tinyxml2::XMLPrinter printer(xmlfile, false, 1);
	
	std::cout << "Starting to generate the plasma project file" << std::endl;
	
	// print the header
	printer.PushHeader(true, true);

	// project element
	printer.OpenElement("PlasmaProject");
	// attribute of project element
	printer.PushAttribute("name", gp->get_projectname());
	
	// models element
	printer.OpenElement("models");

	// model element
	printer.OpenElement("model");
	// attribute of model element
	printer.PushAttribute("name", gp->get_modelname());
	printer.PushAttribute("type", gp->get_modeltype());

	// content element
	printer.OpenElement("content");
	// attribute of content element
	printer.PushText(gp->get_modelcontent());

	// end content element
	printer.CloseElement();
	
	// end model element
	printer.CloseElement();
	
	// end models element
	printer.CloseElement();

	// requirements element
	printer.OpenElement("requirements");

	// loop through all formulas
	int i = 0;
  	for(mpset_t::const_iterator it = gp->all_monitors.begin(); 
    	it != gp->all_monitors.end(); 
    	++it) {
    	// Monitor parameters
    	monitor_params* mp = *it;
    	
    	// requirement element
		printer.OpenElement("requirement");
		// get requirement name from mp
		mp->get_formula_name(&requirementname);
		printer.PushAttribute("name", requirementname.c_str());
		printer.PushAttribute("type", gp->get_requirementtype());

		// content element
		printer.OpenElement("content");
		// attribute of content element
		// get the formula
		mp->get_formula(&requirementformula);
		printer.PushText(requirementformula.c_str());

		// end content element
		printer.CloseElement();

		// end requirement element
		printer.CloseElement();

    	i++;
  	}  
	
	// end requirements element
	printer.CloseElement();

	// resources element
	printer.OpenElement("resources");
	// end resources element
	printer.CloseElement();


	// end project elements
	printer.CloseElement();

	std::cout << "Done generating the plasma project file " << std::endl;

	// release the memory
	if (xmlfile != NULL) {
		 fclose(xmlfile);
	}
}


