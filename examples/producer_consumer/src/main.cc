#include <time.h>
#include "fifo.cc"
#include "consumer.h"
#include "producer.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

// monitor include
#include "monitor.h"


int sc_main(int argc, char *argv[]) {
  const gsl_rng_type *T;
  gsl_rng *r;

  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);

  // seed the generator
  srand(time(NULL));
  gsl_rng_set(r,random());
 
  sc_set_time_resolution(1,SC_NS); // time unit
  Fifo afifo("fifo"); // create a channel fifo

  Producer prod("producer",-1,r);
  Consumer cons("consumer",-1,r);

  prod.out(afifo); // the producer binding
  cons.in(afifo);  // the consumer binding

  
  // Begin - instantiate obs
  mon_observer* obs = local_observer::createInstance(1, 
    &cons, 
    &prod);
  // End - instantiate obs

  // seed the generator
  unsigned long int seed = time(NULL) * getpid();
  sc_set_random_seed(seed);

  sc_start();
  
  gsl_rng_free (r); // release the generator
  return 0;
}
