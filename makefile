CC = g++
OBJS = main.o encrypting.o fileHelper.o
DEBUG = -g
CFLAGS = -Wall -c -static -std=c++11 $(DEBUG) $(INC) $(LIB)
LFLAGS = -Wall -static -std=c++11 $(DEBUG) $(INC) $(LIB)
INC = -I/home/moritz/cppLibraries/boost/boost_1_63_0/
LIB = -L/home/moritz/cppLibraries/boost/boost_1_63_0/stage/lib/ 
LIBOBJS = -lboost_program_options

encryption: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o encryption $(LIBOBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

encrypting.o: encrypting.cpp
	$(CC) $(CFLAGS) encrypting.cpp

fileHelper.o: fileHelper.cpp
	$(CC) $(CFLAGS) fileHelper.cpp

clean: 
	\rm *.o encryption 
