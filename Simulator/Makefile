#set the ODE_DIR variable to point to the base dir of your ODE install
ODE_DIR=../../ode-0.12


CC=g++

DEPS=GlobalFunctions.h
OBJ=GlobalFunctions.o EvolveMorph.o 
	
%.o: %.cpp $(DEPS)
	$(CC) -DdTRIMESH_ENABLED -DdDOUBLE  -I $(ODE_DIR)/include -g -c $< -o $@

EvolveMorph: $(OBJ)
	$(CC) -g -O2 -o $@ $^ -lSM -lICE $(ODE_DIR)/drawstuff/src/.libs/libdrawstuff.a $(ODE_DIR)/ode/src/.libs/libode.a -lGLU -lGL -lglut -lm -lpthread
	
.PHONY: clean	
clean:
	rm -f *.o *~ EvolveMorph
