## compiler
CC = g++
TARGET_ARCH = 	

FLAGS = -O3 -w -Wno-deprecated \
		-DSC_INCLUDE_DYNAMIC_PROCESSES \
		-D_VERBOSE_GLOBAL 

## libraries
TLM_HOME = 	/home/channgo/Tools/pscv/systemc-2.3.0_modified

SYSTEMC_HOME =	/home/channgo/Tools/pscv/systemc-2.3.0_modified

GSL_HOME =	/usr/local/include

CUSTOM_LOCAL = 	/usr/local

VPATH =  /home/channgo/Tools/pscv/examples/control_system/ins

## Include directories
INCLUDE_DIRS = 	$(SYSTEMC_HOME)/include $(GSL_HOME)/gsl ./

INCDIR = $(INCLUDE_DIRS:%=-I %)

## Library directories
LIBRARY_DIRS = 	$(SYSTEMC_HOME)/lib-linux64 $(CUSTOM_LOCAL)/lib

LIBDIR = $(LIBRARY_DIRS:%=-L %)

## SystemC static library
LIB_SC = -lsystemc

## GSL static libraries
LIB_GSL = -lgsl -lgslcblas -lm 

LIBS = $(LIBDIR) $(LIB_SC) $(LIB_GSL)

CFLAGS = $(FLAGS) $(INCDIR)

## Example .cc files
SRCS = $(wildcard ./*.cc )
HDRS = $(wildcard ./*.h  )
OBJS = main.o monitor.o clk_tick.o reward.o utils.o sensor.o actuator.o sa_grp.o proci.o proco.o procm.o bus.o

ecs: $(OBJS) $(HDRS)
	$(CC) -o ecs $(OBJS) $(LIBS)

main.o: main.cc $(HDRS) Makefile $(MAIN_DEPS)

etags:
	find . -name '*.[cc|c|h]' | xargs etags
	find $(INCLUDE_DIRS) -name '*.[cc|c|h]' | xargs etags -a

check:
	@echo $(HDRS)

###################################################
.cpp.o:
	$(CC) $(CFLAGS) -c $< 

.c.o: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.cc.o:	
	$(CC) $(CFLAGS) -c $< -o $@


clean:	
	$(REMOVE)

distclean: clean
	/bin/rm -f \#* ecs

lint:
	lint *.c

REMOVE = /bin/rm -f *~ .*~ *.BAK .*.BAK *.o logfile ecs