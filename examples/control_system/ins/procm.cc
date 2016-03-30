
#line 1 "procm.cc"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_procm_cc__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__

#line 1 "procm.cc"

#line 11 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"
class Automatic;

#line 1 "procm.cc"

#line 16 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifndef __ac_h_
#define __ac_h_
#ifdef __cplusplus
namespace AC {
  typedef const char* Type;
  enum JPType { CALL = 32768, EXECUTION = 65536, CONSTRUCTION = 131072, DESTRUCTION = 262144 };
  enum Protection { PROT_NONE, PROT_PRIVATE, PROT_PROTECTED, PROT_PUBLIC };
  enum Specifiers { SPEC_NONE = 0x0 , SPEC_STATIC = 0x1, SPEC_MUTABLE = 0x2, SPEC_VIRTUAL = 0x4 };
  struct Action {
    void **_args; void *_result; void *_target; void *_that; void *_fptr;
    void (*_wrapper)(Action &);
    inline void trigger () { _wrapper (*this); }
  };
  struct AnyResultBuffer {};
  template <typename T> struct ResultBuffer : public AnyResultBuffer {
    struct { char _array[sizeof (T)]; } _data;
    ~ResultBuffer () { ((T&)_data).T::~T(); }
    operator T& () const { return (T&)_data; }
  };
  template <typename T, typename N> struct TL {
    typedef T type; typedef N next; enum { ARGS = next::ARGS + 1 };
  };
  struct TLE { enum { ARGS = 0 }; };
  template <typename T> struct Referred { typedef T type; };
  template <typename T> struct Referred<T &> { typedef T type; };
  template <typename TL, int I> struct Arg {
    typedef typename Arg<typename TL::next, I - 1>::Type Type;
    typedef typename Referred<Type>::type ReferredType;
  };
  template <typename TL> struct Arg<TL, 0> {
    typedef typename TL::type Type;
    typedef typename Referred<Type>::type ReferredType;
  };
  template <typename T> int ttest(...);
  template <typename T> char ttest(typename T::__TI const volatile *);
  template<typename T> struct HasTypeInfo {
    enum { RET=((sizeof(ttest<T>(0))==1)?1:0) };
  };
  template<typename T, int HAVE = HasTypeInfo<T>::RET> struct TypeInfo {
    enum { AVAILABLE = 0 };
  };
  template<typename T> struct TypeInfo<T, 1> : T::__TI {
    enum { AVAILABLE = 1 };
	 };
  template<typename T> struct RT {};
  template<typename T> RT<T> rt_deduce (const T&) { return RT<T>(); }
  struct Cnv { template<typename T> operator RT<T>() const { return RT<T>(); }};
  #define __AC_TYPEOF(expr) (1?AC::Cnv():AC::rt_deduce(expr))
  template <class Aspect, int Index>
  struct CFlow {
    static int &instance () {
      static int counter = 0;
      return counter;
    }
    CFlow () { instance ()++; }
    ~CFlow () { instance ()--; }
    static bool active () { return instance () > 0; }
  };
}
inline void * operator new (unsigned long int, AC::AnyResultBuffer *p) { return p; }
inline void operator delete (void *, AC::AnyResultBuffer *) { } // for VC++
#endif // __cplusplus
#endif // __ac_h_

#line 1 "procm.cc"
/* Embedded control system modeling in SystemC
 */



#line 1 "procm.h"
/* Embedded control system modeling in SystemC
 */


/* Main processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCM_H
#define PROCM_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 1 "types.h"
/* Embedded control system modeling in SystemC
 */


#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <systemc.h>

// debug flags
//#define PRINT_DEBUG

// cycles skipped limit
const int MAX_COUNT = 4;
// M-of-N sensor system
const int N_SENSOR = 50;
const int M_SENSOR = 37; // the number of working sensor groups >= 37
// M-of-N actuator system
const int N_ACTUATOR = 30;
const int M_ACTUATOR = 27; // the number of working actuator groups >= 27

// rates, mu = 1/lambda
const double time_unit = 1.0;
const double lambda_sensor = 30.0/(30*24*60*60); // 1 month
const double lambda_actuator = 30.0/(2*30*24*60*60); // 2 months
const double lambda_processor = 30.0/(365*24*60*60); // 1 year
const double lambda_cycle = 30.0/2*30; // 1 minute
const double lambda_transient = 30.0/(24*60*60); // 1 day
const double lambda_reboot = 30.0/30; // 30s = time unit

// matrix of transitions
typedef std::vector< std::vector< double > > qmatrix_t;
typedef std::vector< double > tvector_t;

#endif

#line 1 "utils.h"
/* Embedded Control System modeling in SystemC
 */


#ifndef UTILS_H
#define UTILS_H

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 14 "utils.h"
// get max
double get_max(const double x, const double y);

// general discrete distributions
size_t get_discrete(const gsl_rng *r, size_t K, const double *P);

// gaussian distribution
double get_gaussian(const gsl_rng *r, double sigma, double mu);

// bernoulli distribution
int get_bernoulli(const gsl_rng *r, double p);

// exponential distribution
double get_exponential(const gsl_rng *r, double mu);

// get P(X <= x) of exponential distribution
double get_exponential_P(double x, double mu);


#endif

#line 1 "procm.h"

#line 186 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifndef __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__

#line 1 "aspect_definitions.ah"
#ifndef ASPECT_DEFINITIONS_AH
#define ASPECT_DEFINITIONS_AH

#include <systemc>

#line 1 "monitor.h"
/*
 * This code is automatically generated by MAG
 * Copyright (C) 2014 Chan Ngo, INRIA
 * chan.ngo@inria.fr
 */


#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string.h>

/*
 * User objects, forward declaration
 */

class Bus;
class Clk_tick;
class Proci;
class Procm;
class Proco;
class Sa_grp;

/*
 * Monitor class declaration
 */

class monitor0 : public sc_core::mon_prototype {
#line 232 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"
public:
  template <typename, int = 0> struct __BYPASSmonitor0 {};
  template <typename, int> friend struct __BYPASSmonitor0;
private:
#line 34 "monitor.h"


public:

  /*
   * Constructor
   */

  monitor0(sc_core::mon_observer* obs, Clk_tick* obj0, Proci* obj1, Procm* obj2, Proco* obj3, Sa_grp* obj4);

  /*
   * Destructor
   */

  ~monitor0();

  /*
   * Simulate a step of the monitor.
   */

  void step();

  /*
   * Kernel-level callbacks and user-code callbacks
   */

  virtual void callback_event_notified(sc_core::sc_event* event);


  // Value callbacks (if any)


  /*
   * Attributes
   */

private: 

  sc_core::mon_observer* observer;
  mon_status_t status;
  Clk_tick* pnt_clktick;
  Proci* pnt_proci;
  Procm* pnt_procm;
  Proco* pnt_proco;
  Sa_grp* pnt_sa;
  bool tick_notified;

  friend class ::Automatic;

#line 80 "monitor.h"
};



/*
 * This object is derived from mon_observer defined in the SystemC
 * modified kernel. Thus, we can pass it as an argument to the classes
 * implementing the monitors.
 */
class local_observer : public sc_core::mon_observer {
#line 298 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"
public:
  template <typename, int = 0> struct __BYPASSlocal_observer {};
  template <typename, int> friend struct __BYPASSlocal_observer;
private:
#line 89 "monitor.h"


 public:

  // Constructor
  local_observer(unsigned int verbosity, Clk_tick* pnt_clktick, Proci* pnt_proci, Procm* pnt_procm, Proco* pnt_proco, Sa_grp* pnt_sa);

  ~local_observer();

  static local_observer* getInstance();

  static local_observer* createInstance(unsigned int verbosity, Clk_tick* pnt_clktick, Proci* pnt_proci, Procm* pnt_procm, Proco* pnt_proco, Sa_grp* pnt_sa);

 private:

  static local_observer* m_instance;


  friend class ::Automatic;

#line 106 "monitor.h"
};

#endif

#line 7 "aspect_definitions.ah"
#include <iostream>

class Automatic {
#line 333 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"
public:
  template <typename, int = 0> struct __BYPASSAutomatic {};
  template <typename, int> friend struct __BYPASSAutomatic;
private:
#line 9 "aspect_definitions.ah"



  // Declare the monitor objects friends, for access to private data
  
#line 13 "aspect_definitions.ah"


  
#line 17 "aspect_definitions.ah"
; // class




public:
  static Automatic *aspectof () {
    static Automatic __instance;
    return &__instance;
  }
  static Automatic *aspectOf () {
    return aspectof ();
  }
private:

#line 20 "aspect_definitions.ah"
};
#line 366 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

namespace AC {
} 

#line 20 "aspect_definitions.ah"
 // Automatic



#endif

#line 7 "procm.h"
#endif

#line 23 "procm.h"
struct Procm : ::sc_core::sc_module {
#line 383 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

  template <typename, int = 0> struct __BYPASSProcm {};
  template <typename, int> friend struct __BYPASSProcm;
#line 23 "procm.h"

	SC_HAS_PROCESS(Procm);

public:
	// ports
	sc_out < int > timeout;

	// constructor
	Procm(sc_module_name name, int initstate, int initcount, gsl_rng *rnd);
	
	// destructor
	~Procm();

	// transition
	void procm_transition();

	// count transition
	void procm_timeout_transition();
	
	// get current state
	static int get_currentstate();

	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	static int procm_status;
	
	// timeout cycle count
	int count;

	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in current state
	double holdingtime;

	// timeout holding time
	double timeout_holdingtime;
	
	// the random generator
	gsl_rng *r;
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 78 "procm.h"
};

#endif

#line 1 "bus.h"
/* Embedded Control System modeling in SystemC
 */

/* Bus implementation,
 *the communication is modelled using buffers 
 */


#ifndef BUS_H
#define BUS_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 1 "bus.h"

#line 476 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifndef __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif

#line 22 "bus.h"
struct Bus : ::sc_core::sc_module {
#line 485 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

  template <typename, int = 0> struct __BYPASSBus {};
  template <typename, int> friend struct __BYPASSBus;
#line 22 "bus.h"

	SC_HAS_PROCESS(Bus);

public:
	// ports to connect to processors
	sc_in < int > input_reboot;
	sc_in < int > output_reboot;
	sc_in < int > timeout;

	// constructor
	Bus(sc_module_name name, int initcomp, int initreqi, int initreqo, gsl_rng *rnd);
	
	~Bus();

	// transition
	void receiving_input_reboot();
	void receiving_output_reboot();
	void receiving_timeout();

	static int get_comp();

private:
	// module name
	std::string mname;

	// main processor has processed data from input processor
	// and sent corresponding instructions to output 
	// processor (since last timeout), init 1
	static int comp;
	
	// input processor has data ready to send, init 1
	int reqi;
	
	// output processor has instructions ready to be processed, init 0
	int reqo;
	
	// the random generator
	gsl_rng *r;
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 60 "bus.h"
};

#endif

#line 1 "proci.h"
/* Embedded control system modeling in SystemC
 */


/* Input processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCI_H
#define PROCI_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 1 "proci.h"

#line 562 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifndef __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif

#line 24 "proci.h"
struct Proci : ::sc_core::sc_module {
#line 571 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

  template <typename, int = 0> struct __BYPASSProci {};
  template <typename, int> friend struct __BYPASSProci;
#line 24 "proci.h"

	SC_HAS_PROCESS(Proci);

public:
	// ports
	sc_out < int > input_reboot;

	// constructor
	Proci(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Proci();

	// transition
	void proci_transition();
	
	// get current state
	static int get_currentstate();
	
	// get number of reboots
	static int get_number_reboots();

	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	static int proci_status;
	
	// number of reboots
	static int number_reboot;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 76 "proci.h"
};

#endif

#line 1 "proco.h"
/* Embedded control system modeling in SystemC
 */


/* Input processor implement with the exponential distribution
 * It can be either a permanent fault or a transient fault
 * If it is a transient fault, it then reboot 
 */

#ifndef PROCO_H
#define PROCO_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 1 "proco.h"

#line 662 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifndef __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_guard__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif

#line 23 "proco.h"
struct Proco : ::sc_core::sc_module {
#line 671 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

  template <typename, int = 0> struct __BYPASSProco {};
  template <typename, int> friend struct __BYPASSProco;
#line 23 "proco.h"

	SC_HAS_PROCESS(Proco);

public:
	// ports
	sc_out < int > output_reboot;

	// constructor
	Proco(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Proco();

	// transition
	void proco_transition();
	
	// get current state
	static int get_currentstate();
	
	// get number of reboots
	static int get_number_reboots();

	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	static int proco_status;
	
	// number of reboots
	static int number_reboot;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 75 "proco.h"
};

#endif

#line 11 "procm.cc"
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

#line 893 "/home/channgo/Tools/pscv/examples/control_system/ins/procm.cc"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_procm_cc__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_procm_cc__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_procm_cc__
