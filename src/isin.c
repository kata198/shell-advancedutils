/* Copyright (c) 2014-2016 Timothy Savannah All Rights Reserved
 *
 * This code and compiled applications are licensed under the terms of GPLv2.
 *
 * isin - Tests if the element on stdin matches any of args. Returns 0 if match, 1 is no match.
 *
 * notin - Tests if the element on stdin does not match any of args. Returns 0 if match, 1 is no match.
 *
 * Use in bash like:
 *
 *     if echo "--help" | isin $@ ; then
 *         printUsage;
 *         exit;
 *     fi
 *
 * if echo "error" | notin $RESULTS1 $RESULTS2
 * then
 *    doSuccess;
 * fi
 */

/* vim: set ts=4 sw=4 expandtab : */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* We start with 4096 bytes on the stack. If stdin goes beyond that, allocate the max arg length. */
#include <linux/limits.h>
#ifndef ARG_MAX
        #define ARG_MAX 131072
#endif


#define BUFFER_SIZE 4096

/* isin and notin are same source. Compilation is controlled by presence of "PROG_ISIN" or "PROG_NOTIN" being defined. */

#ifndef PROG_ISIN
    #ifndef PROG_NOTIN
        #define PROG_ISIN
    #endif
#endif

#ifdef PROG_ISIN
    #define PROG_NAME_STR "isin"
#endif

#ifdef PROG_NOTIN
    #define PROG_NAME_STR "notin"
#endif


#define EXIT_ERROR -1

int main(int argc, char* argv[])
{
    static char stackBuffer[BUFFER_SIZE];
    char *curPtr;
    char *startPtr;
    size_t spaceRemaining;
    size_t amtRead;
    unsigned int i;

    spaceRemaining = BUFFER_SIZE - 1;

    /* I know &x[0] is meaningless */
    curPtr = startPtr = &stackBuffer[0];
    do {
        errno = 0;
        amtRead = fread(curPtr, 1, spaceRemaining, stdin);
        if(amtRead == 0) {
        /* No more data or error on stream */
                if(errno != 0) {
                        fprintf(stderr, PROG_NAME_STR ": Error %d on input stream ( %s )\n", errno, strerror(errno));
                        /*exit(EXIT_ERROR);*/
                        return EXIT_ERROR;
                }
                break;
        }
        curPtr = &curPtr[amtRead];
        spaceRemaining -= amtRead;
        if ( spaceRemaining <= 0 )
        {
                if ( stackBuffer != startPtr ) {
                        /* If we have already moved from stack to heap, */
                        /*   at this point, more stdin is fed that args can hold, so this obviously is not a match. But print warning to stderr. */
                        #ifdef PROG_ISIN
                            fprintf(stderr, "More than %u bytes of data have been fed to '" PROG_NAME_STR "' on stdin. Argument max is %u bytes. Returning false.\n", ARG_MAX, ARG_MAX);
                            /*exit(1);*/
                            return 1; 
                        #else
                            fprintf(stderr, "More than %u bytes of data have been fed to '" PROG_NAME_STR "' on stdin. Argument max is %u bytes. Returning true.\n", ARG_MAX, ARG_MAX);
                            /*exit(0);*/
                            return 0; 
                        #endif
                }
                char *tmp = malloc(ARG_MAX + 1);
                strcpy(tmp, startPtr);
                startPtr = tmp;
                curPtr = &startPtr[BUFFER_SIZE-1];
                spaceRemaining = ARG_MAX - BUFFER_SIZE;
        }
        
    } while(1);

    /* Strip final newline if present */
    i = strlen(startPtr);
    if ( i > 0 ) {
        if ( startPtr[i-1] == '\n' ) {
            if ( i > 2 && startPtr[i-2] == '\r' )
                startPtr[i-2] = '\0';
            else
                startPtr[i-1] = '\0';
        }
    }
       
    for(i=1; i < argc; i++) {
        #ifdef PROG_ISIN
        if ( strcmp(startPtr, argv[i]) == 0 ) {

            /* Found our match! */

            /*exit(0);*/
            return 0;
        }
        #else
        if ( strcmp(startPtr, argv[i]) == 0 ) {

            /* Found a match, return false! */

            /*exit(1);*/
            return 1;
        }
        #endif

    }
    /* No matches */

    /*exit(1);*/
    #ifdef PROG_ISIN
        return 1;
    #else
        return 0;
    #endif

}
