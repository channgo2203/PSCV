/* Embedded control system modeling in SystemC
 */

#include "proci.h"
#include "sensor.h"
#include "sa_grp.h"


// static instance variable
int Proci::proci_status;
int Proci::number_reboot;


Proci::Proci(sc_module_name name, int initstate, gsl_rng *rnd) {
	
	// initialization
	std::stringstream pname;
	pname << name;
	mname = pname.str();

	n = 3; // number of states
	proci_status = initstate; // init state
	r = rnd; // random generator
	holdingtime = 0;
	number_reboot = 0;

	// the transition rates
	for (int i = 0; i < n; i++) {
		std::vector< double > v(n,0);
		q_matrix.push_back(v);
	}
	q_matrix[1][0] = lambda_processor;
	q_matrix[1][2] = lambda_reboot;  
	q_matrix[2][0] = lambda_processor;
	q_matrix[2][1] = lambda_transient;

	// the transition vector
	probvector.assign(n,0);

	// register the transition function
	SC_THREAD(proci_transition);
}


Proci::~Proci() {

}


void Proci::proci_transition() {
	
	// it is supposed to be run forever
	while(true) {
		
		// calculate the probabilities of all possible transitions
		// from the current state
		tvector_t rates = q_matrix[proci_status];
		double sum_rates = 0;
		// calculate the sum of all transition rates
		for (int i = 0; i < rates.size(); i++) {
			if (rates[i] > 0) {
				// get the transition rate
				sum_rates += rates[i];
			}
		}
		
		// there are some transitions
		if (sum_rates > 0) {
			// calculate the transition probability
			for (int i = 0; i < rates.size(); i++) {
				if (rates[i] > 0) {
					probvector[i] = rates[i]/sum_rates;
				}
				else {
					probvector[i] = 0;
				}
			}
			
			// current state is 2
			if (proci_status == 2) {
				// check the transition guards, if a transition's guard is not satisfied
				// this transition is not allowed meaning the the probability is 0
				int workingsensors = Sa_grp::get_sensors();
				// from 2 --> 0
				if (workingsensors < M_SENSOR) {
					probvector[0] = 0;
				}
				// from 2 --> 1
				if (workingsensors < M_SENSOR) {
					probvector[1] = 0;
				}
			}
			
			// current state is 1
			else if (proci_status == 1) {
				// check the transition guards, if a transition's guard is not satisfied
				// this transition is not allowed meaning the the probability is 0
				int workingsensors = Sa_grp::get_sensors();
				// from 1 --> 0
				if (workingsensors < M_SENSOR) {
					probvector[0] = 0;
				}
				// from 1 --> 2
				if (workingsensors < M_SENSOR) {
					probvector[2] = 0;
				}
			}

			// make the transition
			bool maketransition = false;
			bool isreboot = false;
			for (int i = 0; i < probvector.size(); i++) {
				if (probvector[i] > 0) {
					maketransition = true;
					break;
				}
			}
			if (maketransition) {
				// generate the holding time
				holdingtime = get_exponential(r,1.0/sum_rates);
			
				// advance the simulation time by the holdingtime
				wait(get_max(holdingtime,1.0), SC_MS);

				// choose the transition
				int transition = get_discrete(r,probvector.size(),&probvector[0]);
				#ifdef PRINT_DEBUG
				std::cout << "-----------------------------" << std::endl;
				std::cout << mname << ": current state: " << proci_status << std::endl;
				std::cout << mname << ": sum_rates: " << sum_rates << std::endl;
				std::cout << mname << ": holding time: " << holdingtime << std::endl;
				std::cout << mname << ": probability vector: ";
				for (int i = 0; i < probvector.size(); i++) {
					std::cout << probvector[i] << " ";
				}
				std::cout << std::endl;
				std::cout << mname << ": next state " << transition << std::endl;
				std::cout << "-----------------------------" << std::endl;
				#endif
				
				if (proci_status == 1 && transition == 2) {
					isreboot = true;
				}
				
				proci_status = transition;
				
				if (isreboot) {
					// count the reboot number
					number_reboot = number_reboot + 1;
					// send notification to bus to synchronize
					input_reboot.write(1);
				}
			}
			else {
				wait(time_unit, SC_MS);
			}
		}
		// self transition
		else {
			wait(time_unit, SC_MS);
		}
	}
}

int Proci::get_currentstate() {

	return proci_status;
}


int Proci::get_number_reboots() {

	return number_reboot;
}


int Proci::get_numberofstates() {

	return n;
}


