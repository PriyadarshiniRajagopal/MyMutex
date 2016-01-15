C = gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
#LIB = mythread.a 

CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List all your .cc files here (source files, excluding header files)
SIM_SRC = mytest.c mymutex.c mycond.c mybarrier.c 


# List corresponding compiled object files here (.o files)
SIM_OBJ = mytest.o  mymutex.o mycond.o mybarrier.o
 
#################################

# default rule

all: a4


# rule for making sim_cache

a4 : $(SIM_OBJ)
	$(CC) -o a4 $(CFLAGS) $(SIM_OBJ) -g mythread.a -lm 


# generic rule for converting any .cc file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc


# type "make clean" to remove all .o files plus the sim_cache binary

clean:
	rm -f *.o a4


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f *.o



