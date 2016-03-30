SHELL := /bin/bash

all:
	./mag_run.sh; ./aspectc++_run.sh; cd ./ins; make

check:
	./mag_run.sh; ./aspectc++_run.sh; cd ./ins; make

clean:
	cd ./gen; /bin/rm -rf *~ .*~ *.*; cd ../ins; make clean
