#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <iomanip>

#define MAX_EXE_ORDERS		216
#define EXE_LENGTH		9

int number_exe = -1;
bool max_exe_order_reached = false;
std::vector< std::array<unsigned int,EXE_LENGTH> > exe_book;

/*
 * add an execution to the collection
 */
void add_execution(std::array<unsigned int,EXE_LENGTH> exe) {
	exe_book.push_back(exe);
}

/* 
 * check that an execution is duplicated
 */
bool duplicated_exe(std::array<unsigned int,EXE_LENGTH> exe) {
	bool is_dubplicated = false;
	for (int i = 0; i < exe_book.size(); i++) {
		is_dubplicated = true;
		for (int j = 0; j < exe.size(); j++) {
			if (exe[j] != exe_book[i][j]) {
				is_dubplicated = false;
				break;
			}
		}
		if (is_dubplicated)
			return true;
	}
	
	return false;
}

int main(int argc, char* argv[]){
	unsigned int runs = 0;
	
	// get the parameters
	if (argc == 1) {
		std::cout << "No number of runs is entered, run until reaching possible execution orders" << std::endl;
		max_exe_order_reached = true;
	}
	else if (argc == 2) {
		number_exe = atoi(argv[1]);
		if (number_exe <= 0) {
			std::cout << "Use the default number of run (100)." << std::endl;
			number_exe = 100;
		}
	}
	else {
		std::cout << "Error: usage is sweep_ex3 [runs]" << std::endl;
		exit(-1);
	}
	
	if (max_exe_order_reached) 
		std::cout << "Run until reaching the maximum number of execution orders..." 
		          << std::endl;
	else
		std::cout << "Run ex3 " << number_exe << " times" << std::endl;
	
	// first run
	bool runnable = true;
	char command[6] = "./ex3";
	std::array<unsigned int,EXE_LENGTH> exe_register;
	std::string line;
	std::istringstream strline;
	unsigned int n, index;
		
	system(command);
	runs += 1;
	// access the log file
	std::ifstream exfile("ex.txt",std::ifstream::binary);
  	if (! exfile.good()) {
  		std::cerr << "Unable to open \"ex.txt\" for reading." << std::endl;
    	exit (1);
  	}
  	// read the execution
  	std::getline(exfile,line);
  	strline.str(line);
  	index = 0;
  	while (strline >> n) {
  		if (index < EXE_LENGTH) {
  			exe_register[index] = n;
  			index++;
  		}
  		else break;
  	}
 	// add exe to exe_book
 	add_execution(exe_register);
 	std::cout << "Reading execution: ";
 	for (int i = 0; i < EXE_LENGTH; i++)
 		std::cout << exe_register[i] << " ";
 	std::cout << std::endl;
 	
 	// update runnable
 	if ((number_exe == runs) || (runs == MAX_EXE_ORDERS))
 		runnable = false;
 	
	// run the simulation many times
	while(runnable) {
		std::string line;
		std::istringstream strline;
		unsigned int n, index;
		// run ex3
		system(command);
		runs += 1;
		exfile.clear();
		exfile.seekg(0,std::ifstream::beg);
		// read the execution
  		std::getline(exfile,line);
  		strline.str(line);
  		index = 0;
  		while (strline >> n) {
  			if (index < EXE_LENGTH) {
  				exe_register[index] = n;
  				index++;
  			}
  			else break;
  		}
  		std::cout << "Reading execution: ";
		for (int i = 0; i < EXE_LENGTH; i++)
 			std::cout << exe_register[i] << " ";
		std::cout << std::endl;
		
		if (! duplicated_exe(exe_register)) {
			add_execution(exe_register);
			std::cout << "Adding execution: ";
		 	for (int i = 0; i < EXE_LENGTH; i++)
 				std::cout << exe_register[i] << " ";
		 	std::cout << std::endl;
		}
		// update runnable
		if (max_exe_order_reached) {
			if (exe_book.size() == MAX_EXE_ORDERS)
				runnable = false;
		}
		else {
			if (runs == number_exe)
				runnable = false;
		}
	}
	// file close
  	exfile.close();
	// report results
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "The number of possible execution orders: " << MAX_EXE_ORDERS << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "The number of runs: " << runs << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "The number of different execution orders: " << exe_book.size() << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	float coverage = (float)exe_book.size() / (float)MAX_EXE_ORDERS * 100.0;
	std::cout << "Scheduler coverage (%): " << std::setprecision(3) << coverage << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	float efficient = (float)exe_book.size() / (float)runs * 100.0;
	std::cout << "Dynamic random verification efficiency (%): " << std::setprecision(3) << efficient << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	
	return 0;
}
