## compiler
CC = g++
TARGET_ARCH = 	

FLAGS = -O3 -w -Wno-deprecated \
		-DSC_INCLUDE_DYNAMIC_PROCESSES \
		-D_VERBOSE_GLOBAL 

## libraries
TLM_HOME = 	$(HOME)/Tools/pscv/systemc-2.3.0_modified

SYSTEMC_HOME =	$(HOME)/Tools/pscv/systemc-2.3.0_modified

GSL_HOME =	/usr/local/include

CUSTOM_LOCAL = 	/usr/local

VPATH =  $(HOME)/Tools/pscv/examples/random_scheduler/ex2

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

ex2: $(OBJS) $(HDRS)
	$(CC) -o ex2 $(OBJS) $(LIBS)

main.o: main.cpp $(HDRS) Makefile $(MAIN_DEPS)

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
	/bin/rm -f \#* ex2

lint:
	lint *.c

REMOVE = /bin/rm -f *~ .*~ *.BAK .*.BAK *.o logfile ex2
