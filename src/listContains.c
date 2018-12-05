/**
 * Copyright (c) 2014, 2017, 2018 Timothy Savannah All Rights Reserved
 *
 *  Licensed under terms of the LGPLv2
 */
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void usage(void)
{
    fputs("Usage: listContains [needle] ([haystack...])\n", stderr);
    fputs("   Checks if any items (one item per arg) match needle.\n", stderr);
    fputs("    If so, returns 0 (true). Otherwise, returns 1 (false).\n\n", stderr);
}

int main(int argc, char* argv[])
{
    char *needle;
    char **haystack;
    unsigned int haystackItems;
    int i;

    if ( argc < 3 )
    {
        fputs("Not enough arguments...\n\n", stderr);
        usage();
        return 99;
    }

    needle = argv[1];

    if ( argc == 3 )
    {
        haystack = alloca(sizeof(char *) * 1);
        haystack[0] = argv[2];
        haystackItems = 1;
    }
    else
    {
        haystackItems = argc - 2;

        haystack = alloca(sizeof(char *) * haystackItems);
        for ( i=2; i < argc; i++ )
        {
            haystack[i-2] = argv[i];
        }

    }


    for ( i=0; i < haystackItems; i++)
    {
        if( strcmp(needle, haystack[i]) == 0 )
            return 0;
    }

    return 1;

}
