CFLAGS ?= -O2
CFLAGS := ${CFLAGS} -Wall -pedantic
CC ?= gcc
LDFLAGS ?=

EXECUTABLES = \
	bin/isin \
	bin/notin

all: ${EXECUTABLES}
	
clean:
	rm -f bin/* objects/*

bin/isin: objects/isin.o
	gcc ${LDFLAGS} objects/isin.o -o bin/isin

bin/notin: objects/notin.o
	gcc ${LDFLAGS} objects/notin.o -o bin/notin

objects/isin.o: src/isin.c
	gcc -c ${CFLAGS} -D PROG_ISIN src/isin.c -o objects/isin.o

objects/notin.o: src/notin.c
	gcc -c ${CFLAGS} -D PROG_NOTIN src/notin.c -o objects/notin.o
