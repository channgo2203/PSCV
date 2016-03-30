## compiler
CC = time g++
TARGET_ARCH = 	

FLAGS = -std=c++11 -O3 -w -Wno-deprecated \
		-DSC_INCLUDE_DYNAMIC_PROCESSES \
		-D_VERBOSE_GLOBAL 

## libraries
TLM_HOME = 	/home/chngo/Tools/systemc-2.3.0_modified

SYSTEMC_HOME =	/home/chngo/Tools/systemc-2.3.0_modified

GSL_HOME =	/usr/local/include

CUSTOM_LOCAL = 	/usr/local

VPATH =  /home/chngo/Tools/random_scheduler_test/ex3

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

## Example .cpp files
SRCS = $(wildcard ./*.cpp )
HDRS = $(wildcard ./*.h  )
OBJS = main.o top.o
OBJS_SWEEP = sweep_ex3.o

all: ex3 sweep_ex3

ex3: $(OBJS) $(HDRS)
	$(CC) -o ex3 $(OBJS) $(LIBS)

sweep_ex3: $(OBJS_SWEEP) $(HDRS)
	$(CC) -o sweep_ex3 $(OBJS_SWEEP)
	
main.o: main.cpp $(HDRS) Makefile $(MAIN_DEPS)
sweep_ex3.o: sweep_ex3.cpp $(HDRS) Makefile $(MAIN_DEPS)

etags:
	find . -name '*.[cpp|c|h]' | xargs etags
	find $(INCLUDE_DIRS) -name '*.[cpp|c|h]' | xargs etags -a

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
	/bin/rm -f \#* ex3 sweep_ex3

lint:
	lint *.c

REMOVE = /bin/rm -f *~ .*~ *.BAK .*.BAK *.o logfile ex3 sweep_ex3
