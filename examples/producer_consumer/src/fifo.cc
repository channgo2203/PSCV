/**
 * Defines the functionality of the channel
 **/

#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <systemc.h>
#include "fifo_if.h"

class Fifo : public sc_channel, public fifo_write_if, public fifo_read_if {
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
};

#endif
