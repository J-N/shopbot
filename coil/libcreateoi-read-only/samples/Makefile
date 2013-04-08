# This is a very simple Makefile for compiling COIL sample
# applications. 


CC = gcc -g
INCLUDE = /usr/local/include
LIBS = -pthread -lm -lncurses -lcreateoi -lboost_serialization
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv)

default: sen


all: square drive tracker wander

square: square.c
	$(CC) square.c $(LIBS) -o square

drive: drive.c
	$(CC) drive.c $(LIBS) -o drive
sen: sen.cpp
	g++ -g sen.cpp $(LIBS) -o sen

wander: wander.c
	$(CC) wander.c $(LIBS) $(CVFLAGS) -o wander
	
tracker: tracker.c
	$(CC) tracker.c $(LIBS) $(CVFLAGS) -o tracker

clean:
	rm -f *.o
	rm -f square drive wander tracker sen
