#!/bin/bash

if [ ! -f Simulator/EvolveMorph ]
then
	echo "Must compile Simulator before running!"
	exit -1
fi

if [ ! -f HyperNEAT/out/Hypercube_NEAT ]
then
	echo "Must compile HyperNEAT before running!"
	exit -1
fi

mkdir HyperNEAT/out/data
mkdir HyperNEAT/out/tmp
mkdir HyperNEAT/out/tmp/Files1_1

cd Simulator

#this starts the simulator in the flat ground environment with random seed 1 and thread 1
./container.sh -r 1 -t 1 &
#to run in one of the icy environments set flags -s BLOCK_SPACING and -h BLOCK_HEIGHT
# e.g. ./container.sh -r 1 -t 1 -s 0.05 -h 0.1 &

cd ../HyperNEAT/out

#this starts HyperNEAT with random seed 1 running the experiment defined in experiments/EvolveMorphTrimeshExperiment_recursive_links_max_coord_1_0_sort_first.dat
#and outputting data to data/test.xml
./Hypercube_NEAT -R 1 -I experiments/EvolveMorphTrimeshExperiment_recursive_links_max_coord_1_0_sort_first.dat -O data/test.xml
#look in the HyperNEAT/out/experiments directory for other experiments including the shadow model and multi-objective


