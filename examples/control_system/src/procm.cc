/* Embedded control system modeling in SystemC
 */


#include "procm.h"
#include "bus.h"
#include "proci.h"
#include "proco.h"


// static instance variable
int Procm::procm_status;


/* constructor
 * init_status : 1 - the processor is working
 * initcount : 0 - no cycle is skipped
 * rnd : the random generator 
 */
Procm::Procm(sc_module_name name, int initstate, 
	int initcount, gsl_rng *rnd) {
	
	// initialization
	std::stringstream pname;
	pname << name;
	mname = pname.str();
	
	n = 2; // number of states
	procm_status = initstate; // init state
	count = initcount; // init count
	r = rnd; // random generator
	holdingtime = 0;
	timeout_holdingtime = 0;	

	// the transition rates
	for (int i = 0; i < n; i++) {
		std::vector< double > v(n,0);
		q_matrix.push_back(v);
	}
	q_matrix[1][0] = lambda_processor;

	// the transition vector
	probvector.assign(n,0);

	// working actuator distribution
	SC_THREAD(procm_transition);
	SC_THREAD(procm_timeout_transition);
}

Procm::~Procm() {
	// nothing to clean
}


void Procm::procm_transition() {
	// it is supposed to be run forever
	while(true) {
		// calculate the probabilities of all possible transitions
		// from the current state
		tvector_t rates = q_matrix[procm_status];
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
			std::cout << mname << ": current state: " << procm_status << std::endl;
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
			procm_status = transition;
		}
		// self transition
		else {
			wait(time_unit, SC_MS);
		}
	}
}


void Procm::procm_timeout_transition() {

	while(true) {

		// get current comp from Bus module
 		int comp = Bus::get_comp();

 		// holding time
 		timeout_holdingtime = get_exponential(r,1.0/lambda_cycle);

 		// advance the simulation time by the holdingtime
		wait(get_max(timeout_holdingtime,1.0), SC_MS);

		// the transition probability is 1
		if (comp) {
			count = 0;
			#ifdef PRINT_DEBUG_PROCM
			std::cout << "-----------------------------" << std::endl;
			std::cout << mname << ": timeout_holdingtime " << timeout_holdingtime << std::endl;
			std::cout << mname << ": count is " << count << std::endl;
			std::cout << "-----------------------------" << std::endl;
			#endif
		}
		else {
			if (count > MAX_COUNT)
				count = MAX_COUNT + 1;
			else
				count += 1;

			#ifdef PRINT_DEBUG_PROCM
			std::cout << "-----------------------------" << std::endl;
			std::cout << mname << ": timeout_holdingtime " << timeout_holdingtime << std::endl;
			std::cout << mname << ": count is " << count << std::endl;
			std::cout << "-----------------------------" << std::endl;
			#endif
		}

		// send timeout notification to bus
		timeout.write(1);
	}
}

// get current state
int Procm::get_currentstate() {
	return procm_status;
}

// get number of states
int Procm::get_numberofstates() {
	return n;
}
