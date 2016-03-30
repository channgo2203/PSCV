#!/bin/bash

# path to source code
SOURCE_HOME=/home/channgo/Tools/pscv/examples/control_system/src

# path to mag
MAG_HOME=/home/channgo/Tools/pscv/mag
MAG=$MAG_HOME/mag

# path to folder of generated monitor, aspect-advices, and plasma lab project files
GEN_HOME=/home/channgo/Tools/pscv/examples/control_system/gen

# config file
CONFIG_FILE=$SOURCE_HOME/mm_config.txt

# clean gen folder
cd $GEN_HOME
/bin/rm -f *.ah *.h *.cc *.plasma

# generate the monitor
$MAG -conf $CONFIG_FILE

