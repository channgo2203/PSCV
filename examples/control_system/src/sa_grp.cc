/* Embedded Control System modeling in SystemC
 */


/*
 * Sensors is modelled as a continuous-time Markov chain (CTMC)
 * The mean times that a sensor fails is 24x60x60 time units. 
 * It is distributed exponentially.
 */


#include <iostream>
#include <string.h>
#include "sa_grp.h"



// static instance variable
int Sa_grp::sensors;

int Sa_grp::actuators;


Sa_grp::Sa_grp(sc_module_name name, int sgrps, int agrps, gsl_rng *rnd) {
	
	n_s = sgrps;
	n_a = agrps;
	sensors = sgrps;
	actuators = agrps;

	r = rnd;

	// array of sensor modules
	for (int i = 0; i < n_s; i++) {
		std::stringstream name;
		name << "sensor_" << i;
		s_grps.push_back(new Sensor(name.str().c_str(),3,r));
	}

	// array of actuator modules
	for (int i = 0; i < n_a; i++) {
		std::stringstream name;
		name << "actuator_" << i;
		a_grps.push_back(new Actuator(name.str().c_str(),2,r));
	}

	// register the transition function
	SC_THREAD(sa_grp_transition);
}


Sa_grp::~Sa_grp() {
	
	for (int i = 0; i < s_grps.size(); i++)
		delete s_grps[i];

	for (int i = 0; i < a_grps.size(); i++)
		delete a_grps[i]; 
}


void Sa_grp::sa_grp_transition() {
	// it is supposed to be run forever
	while(true) {
		// calculate the number of working groups
		int s_sum = 0;
		for (int i = 0; i < s_grps.size(); i++) {
			if (s_grps[i]->get_currentstate() >= 2)
				s_sum += 1;
		}
		sensors = s_sum;

		int a_sum = 0;
		for (int i = 0; i < a_grps.size(); i++) {
			if (a_grps[i]->get_currentstate() >= 1)
				a_sum += 1;
		}
		actuators = a_sum;

		// wait for 1 time unit
		wait(time_unit, SC_MS);
	}
}


// get current working sensor groups
int Sa_grp::get_sensors() {
	
	return sensors;

}


// get current working sensor groups
int Sa_grp::get_actuators() {
	
	return actuators;

}


// get number of sensor groups
int Sa_grp::get_sensor_grps() {
	
	return n_s;
}


// get number of actuator groups
int Sa_grp::get_actuator_grps() {
	
	return n_a;
}
