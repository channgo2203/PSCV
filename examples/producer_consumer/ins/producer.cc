
#line 1 "producer.cc"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_producer_cc__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__

#line 1 "producer.cc"

#line 11 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
class Automatic;

#line 1 "producer.cc"

#line 16 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"

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

#line 1 "producer.cc"

#line 1 "producer.h"
/**
 * Defines the functionality of the producer
 **/

#ifndef PRODUCER_H
#define PRODUCER_H

#include <systemc.h>
#include <tlm.h>

#line 1 "fifo.cc"
/**
 * Defines the functionality of the channel
 **/

#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <systemc.h>

#line 1 "fifo_if.h"
#ifndef FIFO_IF
#define FIFO_IF
#include <systemc.h>


class fifo_write_if : virtual public sc_interface {
#line 112 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
public:
  template <typename, int = 0> struct __BYPASSfifo_write_if {};
  template <typename, int> friend struct __BYPASSfifo_write_if;
private:
#line 6 "fifo_if.h"


public:
	virtual void fifo_write(char) = 0;
	virtual void fifo_reset() = 0;	

  friend class ::Automatic;

#line 11 "fifo_if.h"
};

class fifo_read_if : virtual public sc_interface {
#line 130 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
public:
  template <typename, int = 0> struct __BYPASSfifo_read_if {};
  template <typename, int> friend struct __BYPASSfifo_read_if;
private:
#line 13 "fifo_if.h"

public:
	virtual void fifo_read(char&) = 0;
	virtual int fifo_num_available() = 0;

  friend class ::Automatic;

#line 17 "fifo_if.h"
};

#endif

#line 11 "fifo.cc"
class Fifo : public sc_channel, public fifo_write_if, public fifo_read_if {
#line 150 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
public:
  template <typename, int = 0> struct __BYPASSFifo {};
  template <typename, int> friend struct __BYPASSFifo;
private:
#line 11 "fifo.cc"

private:
	enum e {max = 10}; // capacity of the fifo
	char data[max];
	int num_elements, first;
	sc_event write_event, read_event;

public:
	Fifo(sc_module_name name) : sc_channel(name), num_elements(0), first(0) {}
	
	void fifo_write(char c) {
		if (num_elements == max) {
			wait(read_event);
		}

		data[(first + num_elements) % max] = c;
		++num_elements;
		write_event.notify();
	}

	void fifo_read(char &c) {
		if (num_elements ==  0) {
			wait(write_event);
		}
		
		c = data[first];
		--num_elements;
		first = (first + 1) % max;
		read_event.notify();
	}

	void fifo_reset() {
		num_elements = 0;
		first = 0;
	}

	int fifo_num_available() {
		return num_elements;
	}

  friend class ::Automatic;

#line 50 "fifo.cc"
};

#endif

#line 1 "utils.h"
/* Embedded Control System modeling in SystemC
 * Copy right (C) 2015 INRIA, Chan Ngo
 */


#ifndef UTILS_H
#define UTILS_H

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


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
#line 12 "producer.h"
 
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#line 244 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"

#ifndef __ac_guard__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#define __ac_guard__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__

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

class Consumer;
class Producer;

/*
 * Monitor class declaration
 */

class monitor0 : public sc_core::mon_prototype {
#line 286 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
public:
  template <typename, int = 0> struct __BYPASSmonitor0 {};
  template <typename, int> friend struct __BYPASSmonitor0;
private:
#line 30 "monitor.h"


public:

  /*
   * Constructor
   */

  monitor0(sc_core::mon_observer* obs, Consumer* obj0, Producer* obj1);

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

  virtual void callback_timed_notify_phase_end();


  // Value callbacks (if any)


  /*
   * Attributes
   */

private: 

  sc_core::mon_observer* observer;
  mon_status_t status;
  Consumer* pnt_con;
  Producer* pnt_pro;

  friend class ::Automatic;

#line 72 "monitor.h"
};



/*
 * This object is derived from mon_observer defined in the SystemC
 * modified kernel. Thus, we can pass it as an argument to the classes
 * implementing the monitors.
 */
class local_observer : public sc_core::mon_observer {
#line 348 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
public:
  template <typename, int = 0> struct __BYPASSlocal_observer {};
  template <typename, int> friend struct __BYPASSlocal_observer;
private:
#line 81 "monitor.h"


 public:

  // Constructor
  local_observer(unsigned int verbosity, Consumer* pnt_con, Producer* pnt_pro);

  ~local_observer();

  static local_observer* getInstance();

  static local_observer* createInstance(unsigned int verbosity, Consumer* pnt_con, Producer* pnt_pro);

 private:

  static local_observer* m_instance;


  friend class ::Automatic;

#line 98 "monitor.h"
};

#endif

#line 7 "aspect_definitions.ah"
#include <iostream>

class Automatic {
#line 383 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"
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
#line 416 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"

namespace AC {
} 

#line 20 "aspect_definitions.ah"
 // Automatic



#endif

#line 7 "producer.h"
#endif

#line 17 "producer.h"
struct Producer : ::sc_core::sc_module {
#line 433 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"

  template <typename, int = 0> struct __BYPASSProducer {};
  template <typename, int> friend struct __BYPASSProducer;
#line 17 "producer.h"

	SC_HAS_PROCESS(Producer);

public:
   // Definitions of ports
   sc_port<fifo_write_if> out; // output port

   // Constructor
   Producer(sc_module_name name, int c_init, gsl_rng *rnd);

   // Destructor
   ~Producer() {};

   // Definition of processes
   void main();

   // Writing function
   void send(char c);

private:
	int c_int;
	gsl_rng *r;
   private:

#line 16 ""
 friend class monitor0 ;
  friend class ::Automatic;

#line 39 "producer.h"
};

#endif


#line 3 "producer.cc"
Producer::Producer(sc_module_name name, int c_init, gsl_rng *rnd) {
	c_int = c_init;
	r = rnd; // random generator

	SC_THREAD(main);
}

void Producer::send(char c) {
	out->fifo_write(c);
	c_int = c;
}

void Producer::main() {
   	const char* str = "&abcdefgh@";
   	const char* p = str;

	while (true) {
		int b = get_bernoulli(r,0.9);
		if (b) {
			//out->fifo_write(*p);
			//c_int = *p;
			//p++;

			//if(!*p) {
			//	p = str;
			//}
			send(*p);
			p++;
			if (!*p) {
				p = str;
			}
		}
	 	
		wait(1,SC_NS); // waits for 1 nanosecond
	}	
}

#line 510 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/producer.cc"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_producer_cc__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_producer_cc__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_producer_cc__
