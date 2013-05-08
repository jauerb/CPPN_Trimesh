CPPN Trimesh
By Joshua Auerbach
http://www.cs.uvm.edu/~jauerbac
joshua.auerbach@uvm.edu

Documentation for this package is included in this README file.  

-------------
1. LICENSE
-------------

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as published
by the Free Software Foundation (LGPL may be granted upon request). This 
program is distributed in the hope that it will be useful, but without any 
warranty; without even the implied warranty of merchantability or fitness for 
a particular purpose. See the GNU General Public License for more details.

---------------------
2. USAGE and SUPPORT
---------------------

This software is being release in the hopes that it will be useful for others
attempting to evolve the morphologies and controllers of virtual organisms 
(robots) built out of triangular meshes and encoded by Compositional Pattern
Producing Networks (CPPNs).

This software has two main components: an evolutionary algorithm/model based
on HyperNEAT (also known as CPPN-NEAT when not used for evolving ANNs) and
a simulation engine based on the Open Dynamics Engine (ODE).  These two 
components communicate with each other through text files (not ideal, but
sufficiently fast considering the computational bottleneck imposed by
physical simulation.

The software is provided as is; however, we will do our best to maintain it 
and accommodate suggestions. If you want to be notified of future releases of 
the software or have questions, comments, bug reports or suggestions, send
an email to joshua.auerbach@uvm.edu

This code was used for the basis of the experiments reported in 

Auerbach, Joshua E., and Joshua C. Bongard. "On the relationship between 
environmental and morphological complexity in evolved robots." 
In Proceedings of the Fourteenth International Conference on Genetic and 
Evolutionary Computation, pp. 521-528. ACM, 2012.

and

Auerbach, Joshua E., and Joshua C. Bongard. "Environmental influence on the 
evolution of morphological complexity in machines."  In Review, 2013

An example of running one of these experiments is included in the script 
example.sh  

--------------
3. COMPILING
--------------

DEPENDENCIES
--------------
HyperNEAT depends on the TinyXMLDLL Library version 2.0
(http://ucfpawn.homelinux.com/joomla/tinyxmldll/3.html),
JG Template Library (http://sourceforge.net/projects/jgtl/),
Boost c++ libraries (http://www.boost.org/) and
WxWidgets *Only if building in GUI mode* (http://www.wxwidgets.org/).

The Simulator depends on the Open Dynamics Engine (ODE) version 0.12
(http://sourceforge.net/projects/opende/files/ODE/)
When compiling ODE make sure to enable double precision and enable trimesh 
support, e.g. when building run:
./configure --enable-double-precision --with-trimesh=opcode
make

For specific information on how
to build & install those libraries, please see their respective websites.

For further details on compiling and running HyperNEAT please 
refer to HyperNEAT/README.txt

To compile the Simulator open Simulator/Makefile and set the variable
ODE_DIR to be the base location of your ODE install then run
make
(currently this only works on Linux)
