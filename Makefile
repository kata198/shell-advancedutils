CFLAGS ?= -O3 -ffast-math -s
CFLAGS := ${CFLAGS} -Wall -pedantic
CC ?= gcc
LDFLAGS ?= -O1 -s
HOME ?= ${HOME}
PREFIX ?= /usr
DESTDIR ?= "${PREFIX}/bin"


EXECUTABLES = \
	bin/isin \
	bin/isin_nocase \
	bin/notin \
	bin/notin_nocase 

all: ${EXECUTABLES}
	@ echo -e "\n\033[1m Build completed! Run ./install.sh or 'make install' to install into  "'$$'"PREFIX/bin\033[0m\n\n"
	
clean:
	rm -f bin/* objects/*


install: ${EXECUTABLES}
	@ echo -e "\n\033[0m\033[4mInstalling into ${DESTDIR}\033[0m\n"
	mkdir -p ${DESTDIR}
	@ echo -e "\033[91m" >&2
	@ install -m 775 ${EXECUTABLES} -t ${DESTDIR} && \
		 (echo -e "\n\n\033[0m\033[1mExecutables have been installed into ${DESTDIR}. Ensure that location is in your PATH, and you can use them"'!' >&2) \
	   || \
		echo -e "\n\033[1mCould not install to ${DESTDIR}. Please export "'$$'"DESTDIR or "'$$'"PREFIX to an area you can write, or run this as root.\033[0m \033[91m" >&2

	@ echo -e "\033[0m" >&2

bin/isin: objects/isin.o
	gcc ${LDFLAGS} objects/isin.o -o bin/isin

bin/notin: objects/notin.o
	gcc ${LDFLAGS} objects/notin.o -o bin/notin


bin/isin_nocase: objects/isin_nocase.o
	gcc ${LDFLAGS} objects/isin_nocase.o -o bin/isin_nocase

bin/notin_nocase: objects/notin_nocase.o
	gcc ${LDFLAGS} objects/notin_nocase.o -o bin/notin_nocase


objects/isin.o: src/isin.c
	gcc -c ${CFLAGS} -D PROG_ISIN src/isin.c -o objects/isin.o

objects/notin.o: src/notin.c
	gcc -c ${CFLAGS} -D PROG_NOTIN src/notin.c -o objects/notin.o

objects/isin_nocase.o: src/isin.c
	gcc -c ${CFLAGS} -D PROG_ISIN -D CASE_INSENSITIVE src/isin.c -o objects/isin_nocase.o

objects/notin_nocase.o: src/notin.c
	gcc -c ${CFLAGS} -D PROG_NOTIN -D CASE_INSENSITIVE src/notin.c -o objects/notin_nocase.o

