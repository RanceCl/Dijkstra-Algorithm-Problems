# makefile for MP6
# Version: 1
#
# -lm is used to link in the math library
#
# -Wall turns on all warning messages 
#
comp = gcc
comp_flags = -g -Wall 
comp_libs = -lm  

lab6 : graph.o lab6.o
	$(comp) $(comp_flags) graph.o lab6.o -o lab6 $(comp_libs)

graph.o : graph.c graph.h
	$(comp) $(comp_flags) -c graph.c

lab6.o : lab6.c graph.h
	$(comp) $(comp_flags) -c lab6.c

clean :
	rm -f *.o lab6 core

