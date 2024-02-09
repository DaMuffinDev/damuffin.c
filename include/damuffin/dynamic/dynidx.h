#pragma once
#ifndef _DAMUFFIN_C_LIB_DYNAMIC_DYNIDX_H_
#define _DAMUFFIN_C_LIB_DYNAMIC_DYNIDX_H_ 1

#ifndef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_TEMPORARY_CONFIG__DYNIDX
#endif

#include <damuffin/types.h>
#include <stdbool.h>

_T(struct) dynmemidx {
    size_t  nxtsize; // The next size for allocation
    size_t  memsize; // The current memory size
    size_t  endptr;  // The current data pointer
    size_t* block;   // The memory block pointer
} dynidx_t;

static void __dynidx_reserve(dynidx_t*);

dynidx_t dynidx_null();
bool     dynidx_is_null(dynidx_t);

dynidx_t dynidx_new();
void     dynidx_free(dynidx_t*);

dynidx_t dynidx_push(dynidx_t*, size_t);

#ifdef DAMUFFIN_TEMPORARY_CONFIG__DYNIDX
    #undef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #undef DAMUFFIN_TEMPORARY_CONFIG__DYNIDX
#endif

#endif//_DAMUFFIN_C_LIB_DYNAMIC_DYNIDX_H_