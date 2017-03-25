CC = g++
OBJS = main.o
DEBUG = -g
CFLAGS = -Wall -c -static $(DEBUG) $(INC) $(LIB)
LFLAGS = -Wall -static $(DEBUG) $(INC) $(LIB)
INC = -I/home/moritz/cppLibraries/boost/boost_1_63_0/
LIB = -L/home/moritz/cppLibraries/boost/boost_1_63_0/stage/lib/ 
LIBOBJS = -lboost_program_options

encryption: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o encryption $(LIBOBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean: 
	\rm *.o encryption 
