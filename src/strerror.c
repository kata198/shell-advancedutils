/* Copyright (c) 2017 Timothy Savannah All Rights Reserved
 *
 * This code and compiled applications are licensed under the terms of GPLv2.
 *
 */

/* vim: set ts=4 sw=4 expandtab : */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"

static int strtoint(char *str)
{
    int ret;
    char *endptr = { 0 };

    errno = 0;

    if ( likely( *str ) )
    {
        ret = strtol(str, &endptr, 10);
    }
    else
    {
        errno = 1; /* Simulate error */
        return 0;
    }

    if ( unlikely( errno != 0 || ( !ret && endptr == str) || ( *endptr != '\0' ) ) )
    {
        errno = 1;
        return 0;
    }

    return ret;
}

static void printUsage(void)
{
    fputs( \
"Usage:  strerror [exit code]\n" \
" Prints the corrosponding POSIX-defined message for a given exit code ( $? ).\n\n", stderr);

    fputs("  Options:\n\n     --help or --sau-help\t\tPrint this message.\n     --version or --sau-version\t\tPrint version and license information.\n\n", stderr);
    fputs("NOTE: Just because a program returns a code, does NOT mean the code matches the standard meaning.\n" \
"Programs are free to define their own exit codes and meanings.\n\n" \
\
"Standard utils are, however, likely to use the conventional exit codes.\n\n"
, stderr);

}

static void printVersion(void)
{
    printf("strerror version %s\n%s\n\n", SHELL_ADVANCED_UTILS_VERSION, SHELL_ADVANCED_UTILS_COPYRIGHT);
}

int main(int argc, char* argv[])
{
    int exitCode;
    static char *errStr;
    static int i;
    static int varLen;

    /* scan args for --help / --sau-help or --version / --sau-version 
        We support the --sau- prefixes to be compatible with isin/notin
    */
    for( i=1; i < argc; i++ )
    {
        varLen = strlen(argv[i]);

        if( (varLen == 6 && strncmp("--help", argv[i], 6) == 0) ||
               (varLen == 10 && strncmp("--sau-help", argv[i], 10) == 0 ) 
           )
        {
            printUsage();
            return 0;
        }
        else if( (varLen == 9 && strncmp("--version", argv[i], 9) == 0) ||
               (varLen == 13 && strncmp("--sau-version", argv[i], 13) == 0) 
           )
        {
            printVersion();
            return 0;
        }
    }


    if ( unlikely( argc != 2 ) )
    {
        fprintf(stderr, "Error: Wrong number of arguments: Expected 1, got %d\n\n", (argc - 1) );
        printUsage();
        return 1;
    }

    errno = 0;
    exitCode = strtoint(argv[1]);

    if ( unlikely(exitCode == 0 && errno != 0) )
    {
        fprintf(stderr, "Error: Argument is not an integer: \"%s\"\n", argv[1]);
        return 1;
    }

    if ( unlikely(exitCode < 0 || exitCode > 255) )
    {
        fputs("Error: Only exit codes 0-255 are defined.\n", stderr);
        return 1;
    }

    errStr = strerror(exitCode);
    printf("%d - %s\n", exitCode, errStr);

    return 0;
}

/* vim: set ts=4 sw=4 expandtab : */
