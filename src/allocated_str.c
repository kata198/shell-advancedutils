/**
 * Copyright (c) 2017 Timothy Savannah All Rights Reserved
 *
 *  Licensed under terms of the LGPLv2
 */

#include "common.h"
#include "allocated_str.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATED_STRING_MIN_SIZE ( 255 )


STATIC_INLINE_WHEN_STATIC void astr_grow(AllocatedStr *self, int doRealloc)
{
    size_t newSize;

    newSize = self->size * self->growRate;
    if ( unlikely( newSize < ALLOCATED_STRING_MIN_SIZE ) )
    {
        newSize = ALLOCATED_STRING_MIN_SIZE;
    }

    if( unlikely( doRealloc ) && likely ( self->value != NULL ) )
    {
        self->value = realloc(self->value, newSize);
    }
    else
    {
        self->value = malloc(newSize);
        *(self->value) = '\0';
    }

    self->size = newSize;
}


STATIC_INLINE_WHEN_STATIC AllocatedStr *astr_new(size_t initialSize, float growRate)
{
    AllocatedStr *ret;

    if ( growRate <= 0 )
        growRate = ALLOCATED_STR_DEFAULT_GROW_RATE;

    ret = calloc(1, sizeof(AllocatedStr));

    if ( initialSize >= 0 )
    {
        if ( initialSize < ALLOCATED_STRING_MIN_SIZE )
            initialSize = ALLOCATED_STRING_MIN_SIZE;

        ret->value = malloc(initialSize);
        ret->size = initialSize;

        *(ret->value) = '\0';
    }
    ret->growRate = growRate;

    return ret;
}

STATIC_INLINE_WHEN_STATIC void astr_free(AllocatedStr *self)
{
    if ( likely( self->value != (char*)0 ) )
        free(self->value);

    free(self);
}


