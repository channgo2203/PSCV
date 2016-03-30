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
