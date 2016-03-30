/* Embedded control system modeling in SystemC
 */


#include <time.h>
#include <stdio.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

#include <systemc.h>

#include "types.h"
#include "utils.h"
#include "clk_tick.h"
#include "sensor.h"
#include "actuator.h"
#include "sa_grp.h"
#include "proci.h"
#include "proco.h"
#include "procm.h"
#include "bus.h"

// monitor include
#include "monitor.h"


/* 
 * entry point
 */
int sc_main(int argc, char *argv[]) {

	// set simulation time unit
  	sc_set_time_resolution(1, SC_MS);

	const gsl_rng_type *T;
	gsl_rng *r;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);

	// seed the generator
	srand(time(NULL));
	gsl_rng_set(r,random());

	// signals to connect bus and processors
	sc_buffer < int > sig_input_reboot;
	sc_buffer < int > sig_output_reboot;
	sc_buffer < int > sig_timeout;

	// clk tick
	Clk_tick objtick("clk_tick");

	// sensors and actuators
	Sa_grp sagrp("sa_grp",N_SENSOR,N_ACTUATOR,r);

	// input processor
	Proci objproci("proci",2,r);
	objproci.input_reboot(sig_input_reboot);

	// output processor
	Proco objproco("proco",2,r);
	objproco.output_reboot(sig_output_reboot);

	// main processor
	Procm objprocm("procm",1,0,r);
	objprocm.timeout(sig_timeout);

	// bus
	Bus objbus("bus",1,1,0,r);
	objbus.input_reboot(sig_input_reboot);
	objbus.output_reboot(sig_output_reboot);
	objbus.timeout(sig_timeout);

	// Begin - instantiate obs
	mon_observer* obs = local_observer::createInstance(1, 
		&objtick,
		&objproci, 
		&objprocm,
		&objproco,   
		&sagrp);
	// End - instantiate obs
       
        // seed the generator
        unsigned long int seed = time(NULL) * getpid();
        sc_set_random_seed(seed);

	// start the simulator
	sc_start();

	//sc_start (24*60*60,SC_SEC);

	// release the generator
	gsl_rng_free (r);

	return 0;
}
