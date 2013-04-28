# This is a very simple Makefile for compiling our navigation module

CC = gcc -g
INCLUDE = /usr/local/include
LIBS = -pthread -lm -lcreateoi -lboost_serialization

default: navigation

all: navigation

navigation: navigation.cpp
	g++ -g navigation.cpp $(LIBS) -o navigation

clean:
	rm -f *.o
	rm -f navigation
