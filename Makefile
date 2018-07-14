CFLAGS ?= -O3 -ffast-math -s
CFLAGS := ${CFLAGS} -Wall -pedantic
CC ?= gcc
LDFLAGS ?= -Wl,-O1,--sort-common,--as-needed,-z,relro -Wl,-z,combreloc -s
HOME ?= ${HOME}
PREFIX ?= /usr
ETCDIR ?= /etc
DESTDIR ?= 
BINDIR = "${DESTDIR}${PREFIX}/bin"
PROFILE_DIR = "${DESTDIR}${ETCDIR}/profile.d"


EXECUTABLES = \
	bin/isin \
	bin/isin_nocase \
	bin/notin \
	bin/notin_nocase \
	bin/listContains \
	bin/strerror

STANDALONES = \
	utils/aslist \
	utils/bgtask \
	utils/cout \
	utils/dirnamen \
	utils/echoerr \
	utils/gtmp \
	utils/isempty \
	utils/isdigit \
	utils/isfloat \
	utils/nmtmp \
	utils/noecho \
	utils/prefixitems \
	utils/prompt \
	utils/printferr \
	utils/readall \
	utils/sau-activate.sh \
	utils/sortByCol \
	utils/splitContains \
	utils/stripstr \
	utils/strlen \
	utils/lstripstr \
	utils/rstripstr \
	utils/te \
	utils/teq

PROFILE_SCRIPTS=utils/sau-activate.sh

all: ${EXECUTABLES}
	@ echo -e "\n\033[1m Build completed! Run ./install.sh or 'make install' to install into  "'$$'"PREFIX/bin\033[0m\n\n"
	
clean:
	rm -f bin/* objects/*


install: ${EXECUTABLES}
	@ echo -e "\n\033[0m\033[4mInstalling into ${BINDIR}:\033[0m\n"
	@ echo -e "\t\033[1;71m`echo ${EXECUTABLES} ${STANDALONES} | sed 's/ /\n\t/g'`\n\033[0m"
	@ mkdir -p ${BINDIR}
	@ printf "\033[91m" >&2
	@ install -m 775 ${EXECUTABLES} ${STANDALONES} -t ${BINDIR} && \
		 (echo -e "\n\n\033[0m\033[1mExecutables have been installed into ${BINDIR}. Ensure that location is in your PATH, and you can use them"'!' >&2) \
	   || \
		echo -e "\n\033[1mCould not install to ${BINDIR}. Please set "'$$'"DESTDIR or "'$$'"PREFIX to an area you can write (e.x. make install PREFIX="'$$'"HOME), or run this as root.\033[0m \033[91m" >&2
	@ echo -e "\n\033[0m\033[4mInstalling ${PROFILE_SCRIPTS} into ${PROFILE_DIR}\033[0m\n"
	@ mkdir -p ${PROFILE_DIR}
	@ install -m 775 ${PROFILE_SCRIPTS} -t ${PROFILE_DIR} && \
		 (echo -e "\n\n\033[0m\033[1mProfile script has been installed into ${PROFILE_DIR}. Source this file to use advanceds shell utils"'!' >&2) \
	   || \
		echo -e "\n\033[1mCould not install to ${PROFILE_DIR}. Please try again as root, otherwise you must explicitly source ${BINDIR}/`basename ${PROFILE_SCRIPTS}` for some advanced shell util functions. \033[0m \033[91m" >&2
	@ echo -e "\033[0m" >&2


#################################
###  Executables
######################

bin/isin: objects/isin.o
	gcc ${LDFLAGS} objects/isin.o -o bin/isin

bin/notin: objects/notin.o
	gcc ${LDFLAGS} objects/notin.o -o bin/notin

bin/isin_nocase: objects/isin_nocase.o
	gcc ${LDFLAGS} objects/isin_nocase.o -o bin/isin_nocase

bin/notin_nocase: objects/notin_nocase.o
	gcc ${LDFLAGS} objects/notin_nocase.o -o bin/notin_nocase

bin/listContains: objects/listContains.o
	gcc ${LDFLAGS} objects/listContains.o -o bin/listContains

bin/strerror: objects/strerror.o
	gcc ${LDFLAGS} objects/strerror.o -o bin/strerror


#################################
###  Objects
######################

objects/isin.o: src/isin.c src/allocated_str.c src/allocated_str.h src/common.h
	gcc -c ${CFLAGS} -D PROG_ISIN src/isin.c -o objects/isin.o

objects/notin.o: src/notin.c src/allocated_str.c src/allocated_str.h src/common.h
	gcc -c ${CFLAGS} -D PROG_NOTIN src/notin.c -o objects/notin.o

objects/isin_nocase.o: src/isin.c src/common.h
	gcc -c ${CFLAGS} -D PROG_ISIN -D CASE_INSENSITIVE src/isin.c -o objects/isin_nocase.o

objects/notin_nocase.o: src/notin.c src/common.h
	gcc -c ${CFLAGS} -D PROG_NOTIN -D CASE_INSENSITIVE src/notin.c -o objects/notin_nocase.o

objects/listContains.o : src/listContains.c src/common.h
	gcc -c ${CFLAGS} src/listContains.c -o objects/listContains.o

objects/strerror.o : src/strerror.c src/common.h
	gcc -c ${CFLAGS} src/strerror.c -o objects/strerror.o

