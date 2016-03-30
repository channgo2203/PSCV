
#line 1 "actuator.cc"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_actuator_cc__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__

#line 1 "actuator.cc"

#line 11 "/home/channgo/Tools/pscv/examples/control_system/ins/actuator.cc"
class Automatic;

#line 1 "actuator.cc"

#line 16 "/home/channgo/Tools/pscv/examples/control_system/ins/actuator.cc"

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

#line 1 "actuator.cc"
/* Embedded control system modeling in SystemC
 */



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

#line 24 "actuator.h"
SC_MODULE(Actuator) {
#line 187 "/home/channgo/Tools/pscv/examples/control_system/ins/actuator.cc"

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


#line 8 "actuator.cc"
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


#line 350 "/home/channgo/Tools/pscv/examples/control_system/ins/actuator.cc"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_actuator_cc__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_control_system_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_control_system_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_actuator_cc__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_control_system_src_actuator_cc__
