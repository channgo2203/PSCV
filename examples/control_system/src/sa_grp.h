/* Embedded Control System modeling in SystemC
 */

/* Sensors implement the M-of-N systems (N-Modular Redundancy) 
 * meaning that the system fails when fewer than M modules 
 * are funtional
 */


#ifndef SA_GRP_H
#define SA_GRP_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include "types.h"
#include "utils.h"
#include "sensor.h"
#include "actuator.h"


SC_MODULE(Sa_grp) {
	SC_HAS_PROCESS(Sa_grp);

public:
	// constructor
	Sa_grp(sc_module_name name, int sgrps, int agrps, gsl_rng *rnd);

	// destructor
	~Sa_grp();

	// transition
	void sa_grp_transition();

	// get working sensor groups
	static int get_sensors();

	// get working actuator groups
	static int get_actuators();

	// get number of sensor groups
	int get_sensor_grps();

	// get number of actuator groups
	int get_actuator_grps();

private:
	// sensor groups
	std::vector< Sensor* > s_grps;

	// actuator groups
	std::vector< Actuator* > a_grps;
	
	// the number of working sensor groups
	static int sensors;

	// the number of working actuator groups
	static int actuators;
	
	// the number of sensor groups
	int n_s;

	// the number of actuator groups
	int n_a;

	// the random generator
	gsl_rng *r;
};

#endif
