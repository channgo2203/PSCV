
## Compiler
CC	=/usr/bin/g++

## Build options
FLAGS = -g -O3 -Wall -DTIMING \
    -DDEBUG_UTILS \
    -DDEBUG_GLOBAL_PARAMS \
    -DDEBUG_USERVALS \
    -DDEBUG_USERLOCS \
    -DDEBUG_TRACE_GEN \
    -DDEBUG_PLASMA_GEN

## Other local directory
CUSTOM_LOCAL = /usr/local

## Include directories
INCLUDE_DIRS = 	$(CUSTOM_LOCAL)/include \
    ./

INCDIR = $(INCLUDE_DIRS:%=-I %)

## Library directories
LIBRARY_DIRS =	$(CUSTOM_LOCAL)/lib

LIBDIR = $(LIBRARY_DIRS:%=-L %)

## Other static libraries
LIB_OTHERS = 

##LIBS = $(LIBDIR) $(LIB_OTHERS)
LIBS = $(LIBDIR)

CFLAGS = $(FLAGS) $(INCDIR) 

## Source (*.cc) files
SRCS = $(wildcard ./*.cc )

## Header (*.h) files
HDRS = $(wildcard ./*.h  )

## Object files
OBJS = main.o \
	tinyxml2.o \
    monitor_params.o \
    utils.o \
    global_params.o \
    observer.o \
    aspect.o \
    patterns.o \
    mon_gen.o \
    trace_gen.o \
    plasma_gen.o

mag: 	$(OBJS) $(HDRS)
	$(CC) -o $@ $(OBJS) $(LIBS)

main.o: main.cc $(HDRS) Makefile

etags:
	find . -name "*.cc" | xargs etags
	find . -name "*.h" | xargs etags -a 
	find $(INCLUDE_DIRS) -name "*.cc" | xargs etags -a
	find $(INCLUDE_DIRS) -name "*.hh" | xargs etags -a 

check:
	@echo $(HDRS)


# Standard Makefile stuff

.cpp.o:
	$(CC) $(CFLAGS) -c $< 

.c.o: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.cc.o:	
	$(CC) $(CFLAGS) -c $< -o $@


# Clean stuffs
clean:	
	$(REMOVE)

distclean: clean
	/bin/rm -f \#* mag

lint:
	lint *.c

REMOVE = /bin/rm -f *~ .*~ *.BAK .*.BAK *.o logfile mag
