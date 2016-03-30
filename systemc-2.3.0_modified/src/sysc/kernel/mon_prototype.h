#ifndef MON_PROTOTYPE_H
#define MON_PROTOTYPE_H

#include <iostream>


typedef enum{MON_PASS, MON_FAIL, MON_UNDETERMINED} mon_status_t;
namespace sc_core {

class sc_event;


class mon_prototype {

  
  
 public: 
  
  mon_prototype();
  virtual ~mon_prototype();
  
  virtual void callback_init_phase_begin();
  virtual void callback_init_phase_end();
  virtual void callback_init_update_phase_begin();
  virtual void callback_init_update_phase_end();
  virtual void callback_init_delta_notify_phase_begin();
  virtual void callback_init_delta_notify_phase_end();
  virtual void callback_delta_cycle_begin();
  virtual void callback_delta_cycle_end();
  virtual void callback_evaluation_phase_begin();
  virtual void callback_evaluation_phase_end();
  virtual void callback_update_phase_begin();
  virtual void callback_update_phase_end();
  virtual void callback_delta_notify_phase_begin();
  virtual void callback_delta_notify_phase_end();
  virtual void callback_timed_notify_phase_begin();
  virtual void callback_timed_notify_phase_end();
  virtual void callback_thread_suspend();
  virtual void callback_method_suspend();
  virtual void callback_statement_level_trigger(std::string statement);
  virtual void callback_event_notified(sc_event* event);
  virtual const char* to_string() const ;

  int get_id() const;
  virtual const mon_status_t get_status() const;

 protected:
  int mon_id;
  static int num_monitors;
  
};

}; //namespace


#endif
