/* Embedded control system modeling in SystemC
 */


#include "reward.h"


// reward variable to count the times such 
// such that system is up until the shutdown
int rw_up = 0;

bool rw_up_flag = false;


// reward variable to count the times such 
// that system in danger until the shutdown
int rw_danger = 0;

bool rw_danger_flag = false;


// reward variable to count the times 
// such that system is shutdown
int rw_shutdown = 0;

bool rw_shutdown_flag = false;
