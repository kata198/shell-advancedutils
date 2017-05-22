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

#include "common.h"

#define ALLOCATED_STR_STATIC_INCLUDE
#include "allocated_str.h"


/* We start with 4096 bytes on the stack. If stdin goes beyond that, allocate the max arg length. */
#include <linux/limits.h>
#ifndef ARG_MAX
        #define ARG_MAX 131072
#endif


#define INITIAL_BUFFER_SIZE 4096

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

#ifndef CASE_INSENSITIVE
    #define my_strcmp strcmp
#else
    #define my_strcmp strcasecmp
#endif

static AllocatedStr *readData(void)
{
    static AllocatedStr *aStr;

    size_t spaceRemaining;
    size_t totalReadBytes;
    size_t numReadBytes;

    aStr = astr_new(INITIAL_BUFFER_SIZE, 2);

    spaceRemaining = ASTR_SIZE(aStr) - 1;
    totalReadBytes = 0;

    /* I know &x[0] is meaningless */
    do {
        errno = 0;

        numReadBytes = fread(ASTR_VALUE(aStr), 1, spaceRemaining, stdin);
        totalReadBytes += numReadBytes;
        if(numReadBytes == 0) {
        /* No more data or error on stream */
                if( unlikely(errno != 0) ) {
                        fprintf(stderr, PROG_NAME_STR ": Error %d reading from stdin: %s\n", errno, strerror(errno));
                        /* Return an error or partial stream? */
                        if ( totalReadBytes > 0 )
                            return aStr;
                        astr_free(aStr);
                        return NULL;
                }
        }

        if ( feof(stdin) )
            break;

        /* We may not have read all the data we requested, if being piped.
         *   So be explicit */
        if ( spaceRemaining <= 0 )
        {
            astr_grow(aStr, 1);
            spaceRemaining -= ASTR_SIZE(aStr) - totalReadBytes;
        }

    } while(1);

    /* Strip final newline if present */
    if ( ASTR_VALUE(aStr)[totalReadBytes - 1] == '\n' )
        ASTR_VALUE(aStr)[totalReadBytes - 1] = '\0';

    return aStr;
}

#define EXIT_ERROR -1

int main(int argc, char* argv[])
{
    AllocatedStr *inputData;
    int ret;
    int i;
    char *value;

    ret = -1;


    inputData = readData();
    if ( unlikely( inputData == NULL ) )
    {
        fputs("Stream errored before outputting data. Exiting with error code 130", stderr);
        return 130;
    }
    value = ASTR_VALUE(inputData);
       
    for(i=1; i < argc; i++) {
        #ifdef PROG_ISIN
        if ( my_strcmp(argv[i], value) == 0 ) {

            /* Found our match! */

            ret = 0;
            break;
        }
        #else
        if ( my_strcmp(argv[i], value) == 0 ) {

            /* Found a match, return false! */

            ret = 1;
            break;
        }
        #endif

    }
    /* No matches */

    if ( ret == -1 )
    {
    #ifdef PROG_ISIN
        ret = 1;
    #else
        ret = 0;
    #endif
    }

/*cleanup_and_exit:*/
    
    astr_free(inputData);
    return ret;
}
