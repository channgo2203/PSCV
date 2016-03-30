#include "sysc/kernel/mon_prototype.h"

namespace sc_core{

  int mon_prototype::num_monitors = 0;

/**
 * The constructor
 */
mon_prototype::mon_prototype() {
  mon_id = num_monitors++;
}


/**
 * Virtual destructor
 */
mon_prototype::~mon_prototype() {
    
}

int
mon_prototype::get_id() const {
  return mon_id;
}



void mon_prototype::callback_init_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_init_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_init_update_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_init_update_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_init_delta_notify_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_init_delta_notify_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_delta_cycle_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_delta_cycle_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_evaluation_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_evaluation_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_update_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_update_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_delta_notify_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_delta_notify_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_timed_notify_phase_begin(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_timed_notify_phase_end(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_thread_suspend(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_method_suspend(){
  // Virtual function. Implement in deriving class
}
void mon_prototype::callback_statement_level_trigger(std::string statement){
  // Virtual function. Implement in deriving class
}

void mon_prototype::callback_event_notified(sc_event* event){
  // Virtual function. Implement in deriving class
}

const char*
mon_prototype::to_string() const {
  // Virtual
}

const mon_status_t
mon_prototype::get_status() const {
  return MON_FAIL;
}

}; //namespace
 
