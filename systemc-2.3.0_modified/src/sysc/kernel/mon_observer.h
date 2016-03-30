#ifndef MON_OBSERVER_H
#define MON_OBSERVER_H

#include "sysc/kernel/mon_prototype.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_event.h"

//Debugging print
#include <stdarg.h> // for variable argument functions
#include <stdio.h>  // in/out
#include <iostream>
#include <vector>
#include <map>
#include <set>


namespace sc_core {

struct monitor_comp {
  bool operator() (const mon_prototype* lhs, const mon_prototype* rhs) const {
    return lhs->get_id() < rhs->get_id() ;
  }
};

// Set of properties
typedef std::set< mon_prototype*, monitor_comp > monitor_set;
typedef std::pair< monitor_set::const_iterator, bool> monitor_set_pair;
typedef std::vector<mon_prototype*> vect_mon_t;
typedef std::vector< vect_mon_t* > vect_vect_mon_t;

enum sample_point{
  MON_INIT_PHASE_BEGIN,
  MON_INIT_PHASE_END,
  MON_INIT_UPDATE_PHASE_BEGIN,
  MON_INIT_UPDATE_PHASE_END,
  MON_INIT_DELTA_NOTIFY_PHASE_BEGIN,
  MON_INIT_DELTA_NOTIFY_PHASE_END,
  MON_DELTA_CYCLE_BEGIN,
  MON_DELTA_CYCLE_END,
  MON_EVALUATION_PHASE_BEGIN,
  MON_EVALUATION_PHASE_END,
  MON_UPDATE_PHASE_BEGIN,
  MON_UPDATE_PHASE_END,
  MON_DELTA_NOTIFY_PHASE_BEGIN,
  MON_DELTA_NOTIFY_PHASE_END,
  MON_TIMED_NOTIFY_PHASE_BEGIN,
  MON_TIMED_NOTIFY_PHASE_END,
  MON_METHOD_SUSPEND,
  MON_THREAD_SUSPEND,
  MON_INDEX_OF_LAST_SAMPLE_POINT_ELEMENT // To keep track of how many there are
};



class mon_observer {
  
public:

  // Constructor, with verbosity level
  mon_observer(unsigned int verbosity);

  // Destructor
  ~mon_observer();
  
  // Kernel observer
  void init_begin();
  void init_update_begin();
  void init_update_end();
  void init_delta_notification_begin();
  void init_delta_notification_end();
  void init_end();
  void evaluate_begin();
  void evaluate_process_running_begin();
  void evaluate_process_running_end();
  void evaluate_selecting_process();
  void evaluate_end();
  void update_begin();
  void update_selecting_next_channel();
  void update_end();
  void deltanotify_begin();
  void deltanotify_end();
  void timednotify_begin();
  void timednotify_end();
  void method_suspend();
  void thread_suspend();

  void event_notified(sc_event* event);

  // Monitor registration
  void register_monitor(mon_prototype* mon, sample_point inst);

  // Another form, to be used with events
  void register_monitor(mon_prototype* mon, sc_event* evt);

  // These are short-hand for evaluate_phase_begin() and
  // evaluate_phase_end()
  void delta_cycle_begin();
  void delta_cycle_end();

  void _dprint(unsigned int level, const char* format, ...) const;
  void report_status(std::ostream& os);
  
  // Return a pointer to individual properties
  mon_prototype* get_monitor_by_index(unsigned int);

  // For testing purposes we use a vector of identical properties
  vect_mon_t* get_vector_of_monitors_by_index(unsigned int);
  

 protected:
  /* A vector of all properties currently in use */
  vect_mon_t mons;

  /* A vector of vectors of properties (for stress-testing the framework) */
  vect_vect_mon_t mon_vectors;
  
  /* Each simulation object is associated with a simulation context */ 
  sc_simcontext*         m_simc;

  /* Array of sets of monitors. Each monitor needs to be triggered
     when the corresponding sample point is reached. */
  monitor_set** arr_mon_sets;

  std::map<sc_event*, monitor_set*> events_to_monitor_sets;

  unsigned int _debug_level; //for debugging printing



 

}; //class

}; //namespace




#endif
