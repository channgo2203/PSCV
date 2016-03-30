
#line 1 "utils.cc"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_utils_cc__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__

#line 1 "utils.cc"

#line 11 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/utils.cc"
class Automatic;

#line 1 "utils.cc"

#line 16 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/utils.cc"

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

#line 1 "utils.cc"
/* Embedded Control System modeling in SystemC
 * Copy right (C) 2015 INRIA, Chan Ngo
 */



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
#line 9 "utils.cc"
// get max
double get_max(const double x, const double y) {
	if (x > y)
		return x;
	else return y;
}


/*
 * r : the random generator
 * K : size of the outcomes
 * P : array containts the probabilities of the discrete events, 
 * they must be positive
 * output : the discrete random number from 0 to K - 1
 */ 
size_t get_discrete(const gsl_rng *r, size_t K, const double *P) {
	gsl_ran_discrete_t *r_discrete;
	size_t rndvalue;

	r_discrete = gsl_ran_discrete_preproc(K,P);
	rndvalue = gsl_ran_discrete(r,r_discrete);
	// release the memory
	gsl_ran_discrete_free(r_discrete);

	return rndvalue;
}


/*
 * r : the random generator
 * sigma : sigma value of the gaussion distribution
 * mu : mu value of the gaussian distribution
 * rndvalue : the random value of the gaussian distribution
 */
double get_gaussian(const gsl_rng *r, double sigma, double mu) {
	double rndvalue;

	rndvalue = gsl_ran_gaussian(r,sigma);
	rndvalue = rndvalue + mu;

	return rndvalue;
}


/*
 * r : the random generator
 * p : the probability parameter P(0) = 1 - p, P(1) = p
 * rndvalue : the random value of the bernoulli distribution
 */
int get_bernoulli(const gsl_rng *r, double p) {
	int rndvalue;

	rndvalue = gsl_ran_bernoulli(r,p);

	return rndvalue;
}


/*
 * r : the random generator
 * mu : the mean of the distribution = 1 / rate 
 * rndvalue : the random value of the exponential distribution
 */
double get_exponential(const gsl_rng *r, double mu) {
	double rndvalue;

	rndvalue = gsl_ran_exponential(r,mu);

	return rndvalue;
}


double get_exponential_P(double x, double mu) {
	return gsl_cdf_exponential_P(x,mu);
}


#line 201 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/utils.cc"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_utils_cc__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_utils_cc__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_utils_cc__
