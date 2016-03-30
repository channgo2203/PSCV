
#line 1 "sa_grp.h"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_sa_grp_h__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__

#line 1 "sa_grp.h"

#line 11 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"
class Automatic;

#line 1 "sa_grp.h"

#line 16 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

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

#line 1 "sa_grp.h"
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

#line 1 "sensor.h"
/* Embedded Control System modeling in SystemC
 */

/* Sensors implement the M-of-N systems (N-Modular Redundancy) 
 * meaning that the system fails when fewer than M modules 
 * are funtional
 */


#ifndef SENSOR_H
#define SENSOR_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 23 "sensor.h"
SC_MODULE(Sensor) {
#line 201 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

  template <typename, int = 0> struct __BYPASSSensor {};
  template <typename, int> friend struct __BYPASSSensor;
#line 23 "sensor.h"

	SC_HAS_PROCESS(Sensor);

public:
	// constructor
	Sensor(sc_module_name name, int initstate, gsl_rng *rnd);

	// desstructor
	~Sensor();

	// transition
	void sensor_transition();
	
	// get current state
	int get_currentstate();
	
	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	int s;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in the current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;

  friend class ::Automatic;

#line 66 "sensor.h"
};

#endif

#line 1 "actuator.h"
/* Embedded control system modeling in SystemC
 */


/* Actuators implement the M-of-N systems (N-Modular Redundancy) 
 * meaning that the system fails when fewer than M modules 
 * are funtional
 */


#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <systemc.h>
#include <tlm.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 24 "actuator.h"
SC_MODULE(Actuator) {
#line 281 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

  template <typename, int = 0> struct __BYPASSActuator {};
  template <typename, int> friend struct __BYPASSActuator;
#line 24 "actuator.h"

	SC_HAS_PROCESS(Actuator);

public:
	// constructor
	Actuator(sc_module_name name, int initstate, gsl_rng *rnd);
	
	// destructor
	~Actuator();
	
	// transition
	void actuator_transition();
	
	// get current state
	int get_currentstate();
	
	// get number of states
	int get_numberofstates();

private:
	// module name
	std::string mname;
	
	// current states
	int a;
	
	// number of states from 0 ... n-1
	int n;
	
	// transition matrix
	// 0: no transition
	// -lamda: self transition
	// +lamda: the transition rate 
	qmatrix_t q_matrix;
	
	// probability vector
	tvector_t probvector;

	// holding time in the current state
	double holdingtime;
	
	// the random generator
	gsl_rng *r;

  friend class ::Automatic;

#line 67 "actuator.h"
};

#endif


#line 1 "sa_grp.h"

#line 340 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

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
#line 386 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"
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
#line 452 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"
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
#line 487 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"
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
#line 520 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

namespace AC {
} 

#line 20 "aspect_definitions.ah"
 // Automatic



#endif

#line 7 "sa_grp.h"
#endif

#line 26 "sa_grp.h"
struct Sa_grp : ::sc_core::sc_module {
#line 537 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

  template <typename, int = 0> struct __BYPASSSa_grp {};
  template <typename, int> friend struct __BYPASSSa_grp;
#line 26 "sa_grp.h"

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
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 72 "sa_grp.h"
};

#endif

#line 599 "/home/channgo/Tools/pscv/examples/control_system/ins/sa_grp.h"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_sa_grp_h__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_sa_grp_h__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_sa_grp_h__
