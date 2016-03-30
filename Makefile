SHELL := /bin/bash

all:
	cd ./systemc-2.3.0_modified; mkdir objdir; cd ./objdir; export CXX=g++; ../configure; make; sudo make install; cd ../../; cd mag; make

check:
	cd ./systemc-2.3.0_modified; mkdir objdir; cd ./objdir; export CXX=g++; ../configure; make; sudo make install; make check; cd ../../; cd mag; make

clean:
	cd ./systemc-2.3.0_modified/objdir/; make clean; make uninstall; cd ../; rm -rf objdir; cd ../; cd mag; make clean
