
CC = gcc
CXX = g++


INCLUDES = -I../../lab3/part1
CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g -L../../lab3/part1
LDLIBS = -lmylist

libmylist.a: mymdb-lookup
	ar rc libmylist.a mymdb-lookup.o
	ranlib libmylist.a

mymdb-lookup: mymdb-lookup.o mdb.h

mymdb-lookup.o: mdb.h 



.PHONY: clean
clean: 
	rm -f *.o *~ a.out core mymdb-lookup

.PHONY: all
all: clean mymdb-lookup