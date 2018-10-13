CC = g++
OBJS = main.o encrypting.o fileHelper.o cmdLineParser.o
DEBUG = -g
CFLAGS = -Wall -c -static -std=c++11 $(DEBUG) $(INC) $(LIB)
LFLAGS = -Wall -static -std=c++11 $(DEBUG) $(INC) $(LIB)
INC = -I
LIB = -L

encryption: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o encryption $(LIBOBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

encrypting.o: encrypting.cpp
	$(CC) $(CFLAGS) encrypting.cpp

fileHelper.o: fileHelper.cpp
	$(CC) $(CFLAGS) fileHelper.cpp

cmdLineParser.o: cmdLineParser.cpp
	$(CC) $(CFLAGS) cmdLineParser.cpp

clean: 
	\rm *.o encryption 
