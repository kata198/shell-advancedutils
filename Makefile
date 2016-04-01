CFLAGS ?= -O3 -ffast-math -s
CFLAGS := ${CFLAGS} -Wall -pedantic
CC ?= gcc
LDFLAGS ?= -O1 -s
HOME ?= ${HOME}
PREFIX ?= /usr
DESTDIR ?= 
BINDIR = "${DESTDIR}${PREFIX}/bin"


EXECUTABLES = \
	bin/isin \
	bin/isin_nocase \
	bin/notin \
	bin/notin_nocase

STANDALONES = \
	utils/aslist \
	utils/bgtask \
	utils/gtmp \
	utils/nmtmp \
	utils/noecho \
	utils/prompt \
	utils/readall \
	utils/sau-activate.sh \
	utils/sortByCol \
	utils/splitContains

all: ${EXECUTABLES}
	@ echo -e "\n\033[1m Build completed! Run ./install.sh or 'make install' to install into  "'$$'"PREFIX/bin\033[0m\n\n"
	
clean:
	rm -f bin/* objects/*


install: ${EXECUTABLES}
	@ echo -e "\n\033[0m\033[4mInstalling into ${BINDIR}\033[0m\n"
	mkdir -p ${BINDIR}
	@ echo -e "\033[91m" >&2
	@ install -m 775 ${EXECUTABLES} ${STANDALONES} -t ${BINDIR} && \
		 (echo -e "\n\n\033[0m\033[1mExecutables have been installed into ${BINDIR}. Ensure that location is in your PATH, and you can use them"'!' >&2) \
	   || \
		echo -e "\n\033[1mCould not install to ${BINDIR}. Please set "'$$'"DESTDIR or "'$$'"PREFIX to an area you can write (e.x. make install PREFIX="'$$'"HOME), or run this as root.\033[0m \033[91m" >&2

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

