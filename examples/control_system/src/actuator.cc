/* Embedded control system modeling in SystemC
 */


#include "actuator.h"


Actuator::Actuator(sc_module_name name, int initstate, gsl_rng *rnd) {
	
	// initialization
	std::stringstream pname;
	pname << name;
	mname = pname.str();
	
	n = 3; // number of states
	a = initstate; // init state
	r = rnd; // random generator
	holdingtime = 0;

	// the transition rates
	for (int i = 0; i < n; i++) {
		std::vector< double > v(n,0);
		q_matrix.push_back(v);
	}
	q_matrix[1][0] = lambda_actuator;
	q_matrix[2][1] = 2.0*lambda_actuator;

	// the transition vector
	probvector.assign(n,0);

	// working actuator distribution
	SC_THREAD(actuator_transition);
}


Actuator::~Actuator() {
	//nothing to clean
}


void Actuator::actuator_transition() {
	
	// it is supposed to be run forever
	while(true) {
		
		// calculate the probabilities of all possible transitions
		// from the current state
		tvector_t rates = q_matrix[a];
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
			// generate the holding time
			holdingtime = get_exponential(r,1.0/sum_rates);
			
			// advance the simulation time by the holdingtime
			wait(get_max(holdingtime,1.0), SC_MS);

			// calculate the transition probability
			for (int i = 0; i < rates.size(); i++) {
				if (rates[i] > 0) {
					probvector[i] = rates[i]/sum_rates;
				}
				else {
					probvector[i] = 0;
				}
			}

			// make the transition
			int transition = get_discrete(r,probvector.size(),&probvector[0]);
			
			#ifdef PRINT_DEBUG
			std::cout << "-----------------------------" << std::endl;
			std::cout << mname << ": current state: " << a << std::endl;
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

			a = transition;
		}
		// self transition
		else {
			wait(time_unit, SC_MS);
		}
	}
}


int Actuator::get_currentstate() {
	return a;
}


int Actuator::get_numberofstates() {
	return n;
}

