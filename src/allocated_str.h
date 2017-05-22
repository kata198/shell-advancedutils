/**
 * Copyright (c) 2017 Timothy Savannah All Rights Reserved
 *
 *  Licensed under terms of the LGPLv3
 *
 *  allocated_str.h - Header for AllocatedStr functions.
 *    Define ALLOCATED_STR_STATIC_INLINE to make everything static and 
 *    include allocated_str.c into file. If multiple files and you want
 *    this, consider making a use_allocated_str.c that just has the include,
 *    compiling into an object and linking to final binary
 */
#ifndef __ALLOCATED_STR_H
#define __ALLOCATED_STR_H

#include <sys/types.h>

#ifdef ALLOCATED_STR_STATIC_INCLUDE
  #define STATIC_INLINE_WHEN_STATIC static inline
#else
  #define STATIC_INLINE_WHEN_STATIC
#endif

#define ALLOCATED_STR_DEFAULT_GROW_RATE 2

typedef struct {
    char *value;
    size_t size;
    float growRate;
} AllocatedStr;

#define ASTR_VALUE(_a_str) (_a_str->value)
#define ASTR_SIZE(_a_str)  (_a_str->size)

#define ASTR_LEN_OK(_a_str, _len) ( ASTR_SIZE(_a_str) >= _len )

STATIC_INLINE_WHEN_STATIC AllocatedStr *astr_new(size_t initialSize, float growRate);

STATIC_INLINE_WHEN_STATIC void astr_free(AllocatedStr *self);

STATIC_INLINE_WHEN_STATIC void astr_grow(AllocatedStr *self, int doRealloc);

static inline void astr_ensureSize(AllocatedStr *self, size_t newSize, int doRealloc)
{
    while ( ! ASTR_LEN_OK(self, newSize) )
    {
        astr_grow(self, doRealloc);
    }
}


#ifdef ALLOCATED_STR_STATIC_INCLUDE
#include "allocated_str.c"
#endif

#endif
