#ifndef FIFO_IF
#define FIFO_IF
#include <systemc.h>


class fifo_write_if : virtual public sc_interface {

public:
	virtual void fifo_write(char) = 0;
	virtual void fifo_reset() = 0;	
};

class fifo_read_if : virtual public sc_interface {
public:
	virtual void fifo_read(char&) = 0;
	virtual int fifo_num_available() = 0;
};

#endif
