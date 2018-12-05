/**
 * Copyright (c) 2014-2017 Timothy Savannah All Rights Reserved
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

static void printUsage(void)
{
    fputs("Usage:  " PROG_NAME_STR  " [test_val1] ([test_val..N])\n  Tests input from stdin (minus trailing newline) against a list of values,\n   given as commandline arguments.\n\n", stderr);
    fputs("  Options:\n\n     --sau-help\t\t\tPrint this message.\n     --sau-version\t\tPrint version and license information.\n\n", stderr);
    #ifdef PROG_ISIN
      fputs("isin  - returns 0 (true) if string read from stdin matches one of the provided arguments.\n", stderr);
      fputs("      * returns 1 (false) if string read from stdin does NOT match one of the provided arguments.\n", stderr);
      fputs("      X returns > 127 if an error was encountered that prevented a valid test from occuring.\n", stderr);
    #else
      fputs("notin - returns 0 (true) if string read from stdin does NOT match one of the provided arguments.\n", stderr);
      fputs("      * returns 1 (false) if string read from stdin matches one of the provided arguments.\n", stderr);
      fputs("      X returns > 127 if an error was encountered that prevented a valid test from occuring.\n", stderr);
    #endif
    fputs("\nExample:\techo \"$arg\" | "  PROG_NAME_STR  "  '-s' '--something'\n\n", stderr);
}

static void printVersion(void)
{
    printf("%s version %s\n%s\n\n", PROG_NAME_STR, SHELL_ADVANCED_UTILS_VERSION, SHELL_ADVANCED_UTILS_COPYRIGHT);
}

int main(int argc, char* argv[])
{
    static AllocatedStr *inputData;
    int ret;
    static int i, varLen;
    static char *value;

    if ( argc < 2 )
    {
        printUsage();
        return 128;
    }

    
    for( i=1; i < argc; i++ )
    {
        varLen = strlen(argv[i]);

        if( (varLen == 10 && strncmp("--sau-help", argv[i], 10) == 0) )
        {
            printUsage();
            return 0;
        }
        else if( (varLen == 13 && strncmp("--sau-version", argv[i], 13) == 0) )
        {
            printVersion();
            return 0;
        }
    }

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
