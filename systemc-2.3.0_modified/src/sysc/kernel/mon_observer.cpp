#include "sysc/kernel/mon_observer.h"




namespace sc_core {

  /**
   * Constructor with verbosity level
   */
mon_observer::mon_observer(unsigned int verbosity) {
  _debug_level = verbosity;
  
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(6, "k::mon_observer::%s() called\n", __FUNCTION__);
#endif

  m_simc = sc_get_curr_simcontext();
  
  // Attach the observer to the sc_simcontext
  m_simc->attach_observer(this);

  // Initialize the sets
  arr_mon_sets = new monitor_set* [MON_INDEX_OF_LAST_SAMPLE_POINT_ELEMENT];
  for (int i = 0; i < MON_INDEX_OF_LAST_SAMPLE_POINT_ELEMENT; i++) {
    arr_mon_sets[i] = new monitor_set();
  }

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(6, "k::mon_observer::%s() returns\n", __FUNCTION__);  
#endif


}

// Destructor
mon_observer::~mon_observer() {
	//delete mons
	for (int i = 0; i < mons.size(); i++) {
		delete mons[i];		
	}
	
	//delete arr_mon_sets
	for (int i = 0; i < MON_INDEX_OF_LAST_SAMPLE_POINT_ELEMENT; i++) {
		if (arr_mon_sets[i]) {
			for (std::set< mon_prototype*, monitor_comp >::iterator it=arr_mon_sets[i]->begin(); it!=arr_mon_sets[i]->end(); ++it)
				delete (*it);
		}
	}
}

void
mon_observer::report_status(std::ostream& os) {
  os << "MON_OBSERVER: overwrite report_status() in the local observer" << std::endl;
}



mon_prototype*
mon_observer::get_monitor_by_index(unsigned int index) {
  assert(index < mons.size());
  return mons[index]; 
}


/**
 * For empirical analysis we need more than one instance of the same
 * monitor. Thus we use a vector of monitors for each monitor. This
 * function returns the appropriate vector for each monitor.
 */
vect_mon_t*
mon_observer::get_vector_of_monitors_by_index(unsigned int index) {
  return mon_vectors[index];
}


void 
mon_observer::init_begin(){

#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_PHASE_BEGIN]->size();

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_INIT_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;

#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_phase_begin();

#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}


void 
mon_observer::init_update_begin(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_UPDATE_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_UPDATE_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_INIT_UPDATE_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_update_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}



void 
mon_observer::init_update_end(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_UPDATE_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_UPDATE_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_INIT_UPDATE_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_update_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}



void 
mon_observer::init_delta_notification_begin(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_delta_notify_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}



void 
mon_observer::init_delta_notification_end(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_INIT_DELTA_NOTIFY_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_delta_notify_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}



void 
mon_observer::init_end(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_INIT_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_INIT_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_INIT_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_init_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}




void 
mon_observer::delta_cycle_begin() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_DELTA_CYCLE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_DELTA_CYCLE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_DELTA_CYCLE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_delta_cycle_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}


void 
mon_observer::delta_cycle_end() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_DELTA_CYCLE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_DELTA_CYCLE_END]->begin(); 
	 it != arr_mon_sets[MON_DELTA_CYCLE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_delta_cycle_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void 
mon_observer::evaluate_begin() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  // According to the current simulation semantics, a new evaluation
  // phase also starts a new delta cycle. If this changes in future
  // implementations, the line below should be changed as needed.
  delta_cycle_begin();

  unsigned int num_elements = arr_mon_sets[MON_EVALUATION_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_EVALUATION_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_EVALUATION_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_evaluation_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
  
}

void
mon_observer::evaluate_end() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  unsigned int num_elements = arr_mon_sets[MON_EVALUATION_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_EVALUATION_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_EVALUATION_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_evaluation_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void 
mon_observer::update_begin() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_UPDATE_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_UPDATE_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_UPDATE_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_update_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}
  
void
mon_observer::update_end() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_UPDATE_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_UPDATE_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_UPDATE_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_update_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
  
}

void 
mon_observer::deltanotify_begin() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_DELTA_NOTIFY_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_DELTA_NOTIFY_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_DELTA_NOTIFY_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_delta_notify_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void
mon_observer::deltanotify_end() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_DELTA_NOTIFY_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_DELTA_NOTIFY_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_DELTA_NOTIFY_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_delta_notify_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }

  // According to the current simulation semantics, each delta
  // notification phase ends the current delta cycle. If this changes
  // in future implementations, the line below should be changed as
  // needed.
  delta_cycle_end();

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void 
mon_observer::timednotify_begin() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_TIMED_NOTIFY_PHASE_BEGIN]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_TIMED_NOTIFY_PHASE_BEGIN]->begin(); 
	 it != arr_mon_sets[MON_TIMED_NOTIFY_PHASE_BEGIN]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_timed_notify_phase_begin();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }
  
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void
mon_observer::timednotify_end() {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_TIMED_NOTIFY_PHASE_END]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_TIMED_NOTIFY_PHASE_END]->begin(); 
	 it != arr_mon_sets[MON_TIMED_NOTIFY_PHASE_END]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_timed_notify_phase_end();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}

void 
mon_observer::method_suspend(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_METHOD_SUSPEND]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_METHOD_SUSPEND]->begin(); 
	 it != arr_mon_sets[MON_METHOD_SUSPEND]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_method_suspend();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
 }

void 
mon_observer::thread_suspend(){
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
    
  unsigned int num_elements = arr_mon_sets[MON_THREAD_SUSPEND]->size();
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", num_elements);
#endif

  if (num_elements > 0) {
    monitor_set::const_iterator it;
    for (it  = arr_mon_sets[MON_THREAD_SUSPEND]->begin(); 
	 it != arr_mon_sets[MON_THREAD_SUSPEND]->end(); 
	 it++) {
    
      mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
      mp->callback_thread_suspend();
#ifdef DEBUG_PRINT_IN_SCHEDULER
      _dprint(9, "Monitor callback returned\n");
#endif
    }
  }


#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
 }


void
mon_observer::event_notified(sc_event* event) {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
  monitor_set* ms = events_to_monitor_sets[event];

#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "There are %d monitors in the queue\n", ms->size());
#endif
  
  assert (ms->size() > 0);
  
  monitor_set::const_iterator it;
  for (it  = ms->begin(); 
       it != ms->end(); 
       it++) {
    
    mon_prototype* mp = *it;
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(9, "Calling back monitor %s\n", mp->to_string());
#endif
    
    mp->callback_event_notified(event);
    
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(9, "Monitor callback returned\n");
#endif
  }
  
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
  
}


  // Monitor registration for kernel-level sample points
void
mon_observer::register_monitor(mon_prototype* mon, sample_point obs) {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
#endif
  
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(8, "Registering monitor %s\n", mon->to_string());
#endif

  monitor_set_pair p = arr_mon_sets[obs]->insert(mon);
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(9, "Monitor %s inserted%s.\n", (p.second ? "was" : "was not" ), (p.second ? " successfully" : "" ));
#endif
   
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}


  // Monitor registration for events
void
mon_observer::register_monitor(mon_prototype* mon, sc_event* eve) {
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() called\n", __FUNCTION__);
  _dprint(8, "Registering monitor %s\n", mon->to_string());
#endif
  
  // Get the set of monitors to be alerted for this event notification
  monitor_set* ms = events_to_monitor_sets[eve];

  // Check if there are prior monitors already listed
  if (ms == 0) {
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(8, "The set of monitors for this event is currently empty\n");
#endif

    monitor_set* newms = new monitor_set();
    eve->register_observer(this);
    monitor_set_pair p = newms->insert(mon);
    events_to_monitor_sets[eve] = newms;
    assert (p.second);
  }

  else {
#ifdef DEBUG_PRINT_IN_SCHEDULER
    _dprint(8, "The set of monitors for this event already has %d monitors\n", ms->size());
#endif
    
    monitor_set_pair p = ms->insert(mon);
    assert(p.second);
  }

  
   
#ifdef DEBUG_PRINT_IN_SCHEDULER
  _dprint(7, "k::mon_observer: %s() returns\n", __FUNCTION__);
#endif
}



/**
 * Debugging print
 */
void 
mon_observer::_dprint(unsigned int level, const char* format, ...) const {
     va_list arg_list;
    
     va_start(arg_list, format); // initialize the vararg list
     
     if (_debug_level >= level) {
       for (unsigned int i=0; i<level; i++)
	 printf(" ");
       
       vprintf(format, arg_list);
     }
     
     va_end(arg_list); //clean up
   }
   
}; //namespace
