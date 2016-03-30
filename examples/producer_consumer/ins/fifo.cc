
#line 1 "fifo.cc"
#ifndef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#define __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_fifo_cc__
#endif // __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__

#line 1 "fifo.cc"

#line 11 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"
class Automatic;

#line 1 "fifo.cc"

#line 16 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"

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
#line 99 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"
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
#line 117 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"
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
#line 137 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"
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

#line 190 "/home/channgo/Tools/pscv/examples/producer_consumer/ins/fifo.cc"

#ifdef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_fifo_cc__
#ifdef __ac_need__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#ifndef __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#define __ac_have__home_channgo_Tools_pscv_examples_producer_consumer_src_aspect_definitions_ah__
#include "aspect_definitions.ah"
#endif
#endif
#undef __ac_FIRST__home_channgo_Tools_pscv_examples_producer_consumer_src__
#undef __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_fifo_cc__
#endif // __ac_FIRST_FILE__home_channgo_Tools_pscv_examples_producer_consumer_src_fifo_cc__
