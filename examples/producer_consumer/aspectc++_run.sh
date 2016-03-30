#!/bin/bash

# path to aspectc++
ASPECTC_HOME=/home/channgo/Tools/pscv/aspectc++
AC=$ASPECTC_HOME/ac++
AG=$ASPECTC_HOME/ag++

# path to systemc
SYSTEMC_HOME=/home/channgo/Tools/pscv/systemc-2.3.0_modified

# path to source code
SOURCE_HOME=/home/channgo/Tools/pscv/examples/producer_consumer/src

# path to instrumented code
INST_HOME=/home/channgo/Tools/pscv/examples/producer_consumer/ins

# path to folder of generated monitor, aspect-advices, and plasma lab project files
GEN_HOME=/home/channgo/Tools/pscv/examples/producer_consumer/gen

# clean instrument folder
cd $INST_HOME
/bin/rm -f *.h *.cc

# copy monitor and aspect-advice files to source folder
cd $GEN_HOME
yes | cp -rf *.h *.cc *.ah $SOURCE_HOME/

# generate puma.config
cd $ASPECTC_HOME
$AG --gen_config

# instrument code
cd $SOURCE_HOME

for i in *.h ; do
	[[ -f "$i" ]] || continue
	$AC -c "$i" -o $INST_HOME/"$i" -p $SOURCE_HOME/ -I $SOURCE_HOME -I $SYSTEMC_HOME/include --config $ASPECTC_HOME/puma.config
done

for i in *.cc ; do
	[[ -f "$i" ]] || continue
	$AC -c "$i" -o $INST_HOME/"$i" -p $SOURCE_HOME/ -I $SOURCE_HOME -I $SYSTEMC_HOME/include --config $ASPECTC_HOME/puma.config
done