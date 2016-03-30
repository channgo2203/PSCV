/* Embedded control system modeling in SystemC
 */


/*
 * Bus is model as a module that communicated with 
 * the main, input, and output processors to when 
 * the timeout, input reboot, and output reboot 
 * are notified, respectively.
 */


#include "bus.h"
#include "sensor.h"
#include "actuator.h"
#include "proci.h"
#include "proco.h"
#include "procm.h"
#include "sa_grp.h"




// static instance variable
int Bus::comp;


/*
 * constructor
 * name : name of the module
 * initcomp : 1 means that main processor is done its work
 * initreqi : 1 means that input processor has datat to  send
 * initreqo : 0 means that output processor has no instructions
 * rnd : the random generator
 */
Bus::Bus(sc_module_name name, int initcomp, int initreqi, 
	int initreqo, gsl_rng *rnd) {

	// initialization
	std::stringstream pname;
	pname << name;
	mname = pname.str();

	comp = initcomp;
	reqi = initreqi;
	reqo = initreqo;
	r = rnd;

	// notified when receiving input_reboot
	SC_METHOD(receiving_input_reboot);
	sensitive << input_reboot;
	dont_initialize();

	// notified when receiving output_reboot
	SC_METHOD(receiving_output_reboot);
	sensitive << output_reboot;
	dont_initialize();

	// notified when receiving timeout
	SC_METHOD(receiving_timeout);
	sensitive << timeout;
	dont_initialize();
}
	

Bus::~Bus() {
	// nothing to clean up
}


void Bus::receiving_input_reboot() {
		// procm status
		int procm_status = Procm::get_currentstate();
		// performs a computation if has already done so or
		// it is up and ouput clear (i.e. nothing waiting)
		if ((comp == 1) || ((procm_status == 1) && (reqo == 0))) {
			comp = 1;
		}
		else {
			comp = 0;
		}
		
		// up therefore something to process
		reqi = 1;
		
		// something to process if not functioning and either
		// there is something already pending
		// or the main processor sends a request
		int proco_status = Proco::get_currentstate();
		// working actuators
		int workingactuators = Sa_grp::get_actuators(); 
		if(!(proco_status == 2 && workingactuators >= M_ACTUATOR) && (reqo == 1 || procm_status == 1)) {
			reqo = 1;
		}
		else {
			reqo = 0;
		}

		#ifdef PRINT_DEBUG
		std::cout << "-----------------------------" << std::endl;
		std::cout << mname << "input_reboot: procm_status is " << procm_status << std::endl;
		std::cout << mname << "input_reboot: proco_status is " << proco_status << std::endl;
		std::cout << mname << "input_reboot: workingactuators is " << workingactuators << std::endl;
		std::cout << mname << "input_reboot: comp is " << comp << std::endl;
		std::cout << mname << "input_reboot: reqi is " << reqi << std::endl;
		std::cout << mname << "input_reboot: reqo is " << reqo << std::endl;
		std::cout << "-----------------------------" << std::endl;
		#endif
}


void Bus::receiving_output_reboot() {
		// procm status
		int procm_status = Procm::get_currentstate();
		// performs a computation if it has already or
		// something waiting and is up
		// (can be processes as the output has come up and cleared pending requests)
		if ((comp == 1) || ((procm_status == 1) && (reqi == 1))) {
			comp = 1;
		}
		else {
			comp = 0;
		}
		// something to process it they are up or
		// there was already something and the main processor acts
		// (output now up must be due to main processor being down)
		int proci_status = Proci::get_currentstate();
		// working sensors
		int workingsensors = Sa_grp::get_sensors(); 
		if((proci_status == 2 && workingsensors >= M_SENSOR) || (reqi == 1 && procm_status == 0)) {
			reqi = 1;
		}
		else {
			reqi = 0;
		}
		// output and actuators up therefore nothing can be pending
		reqo = 0;

		
		#ifdef PRINT_DEBUG
		std::cout << "-----------------------------" << std::endl;
		std::cout << mname << "output_reboot: procm_status is " << procm_status << std::endl;
		std::cout << mname << "output_reboot: proci_status is " << proci_status << std::endl;
		std::cout << mname << "output_reboot: workingsensors is " << workingsensors << std::endl;
		std::cout << mname << "output_reboot: comp is " << comp << std::endl;
		std::cout << mname << "output_reboot: reqi is " << reqi << std::endl;
		std::cout << mname << "output_reboot: reqo is " << reqo << std::endl;
		std::cout << "-----------------------------" << std::endl;
		#endif
		
}


void Bus::receiving_timeout() {
		// procm status
		int procm_status = Procm::get_currentstate();
		// performs a computation if it is up something was pending
		// and nothing is waiting for the output
		if (reqi == 1 && reqo == 0 && procm_status == 1) {
			comp = 1;
		}
		else {
			comp = 0;
			/*
			#ifdef PRINT_DEBUG
			std::cout << "-----------------------------" << std::endl;
			std::cout << mname << "timeout: reqi is " << reqi << std::endl;
			std::cout << mname << "timeout: reqo is " << reqo << std::endl;
			std::cout << mname << "timeout: procm_status is " << procm_status << std::endl;
			std::cout << mname << "timeout: comp is " << comp << std::endl;
			std::cout << "-----------------------------" << std::endl;
			#endif
			*/
		}
		// something to process if up or
		// already something and main process cannot act 
		// (down or outputs pending)
		int proci_status = Proci::get_currentstate();
		// working sensors
		int workingsensors = Sa_grp::get_sensors(); 
		if((proci_status == 2 && workingsensors >= M_SENSOR) || 
			(reqi == 1 && (procm_status == 0 || reqo == 1))) {
			reqi = 1; // input processor has some data to send
		}
		else {
			reqi = 0; // input processor has no data to send
		}
		// something to process if they are not functioning and 
		// either something is already pending
		// or the main processor acts
		int proco_status = Proco::get_currentstate();
		// working actuators
		int workingactuators = Sa_grp::get_actuators(); 
		if(!(proco_status == 2 && workingactuators >= M_ACTUATOR) 
			&& (reqo == 1 || (procm_status == 1 && reqi == 1))) {
			reqo = 1; // output processor has some instructions
		}
		else {
			reqo = 0; // no instructions
		}
}

int Bus::get_comp() {

	return comp;
}

