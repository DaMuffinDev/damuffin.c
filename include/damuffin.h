#pragma once
#ifndef _DAMUFFIN_C_LIB_H_
#define _DAMUFFIN_C_LIB_H_ 1

#include <damuffin/config.h>
#include <damuffin/util.h>

#ifdef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #include <damuffin/types.h>
#endif//DAMUFFIN_CONFIG_TYPE_SHORTHANDS

#ifdef DAMUFFIN_CONFIG_MAIN_SHORTHANDS
    #ifdef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
        #define MAIN(a, b)       i32 main(i32 a, char* b[])
        #define EMPTY_MAIN(code) i32 main() {return code;}
        #define DEFAULT_MAIN     MAIN(argc, argv)
    #else
        #define MAIN(a, b)       int main(int a, char* b[])
        #define EMPTY_MAIN(code) int main() {return code;}
        #define DEFAULT_MAIN     MAIN(argc, argv)
    #endif//DAMUFFIN_CONFIG_TYPE_SHORTHANDS
#endif//DAMUFFIN_CONFIG_MAIN_SHORTHANDS

#ifdef DAMUFFIN_CONFIG_STDLIB_MALLOC_SHORTHANDS
    #define Malloc(T, n)    (T*)malloc(sizeof(T) * n)
    #define SingleMalloc(T) (T*)malloc(sizeof(T))

    #define VarMalloc(Var, T, n)    T* Var = Malloc(T, n)
    #define SingleVarMalloc(Var, T) T* Var = SingleMalloc(T)
#endif//DAMUFFIN_CONFIG_STDLIB_MALLOC_SHORTHANDS

#ifdef DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS
    #define Calloc(T, n)    (T*)calloc(n, sizeof(T))
    #define SingleCalloc(T) (T*)calloc(1, sizeof(T))

    #define VarCalloc(Var, T, n)    T* Var = Calloc(T, n)
    #define SingleVarCalloc(Var, T) T* Var = SingleCalloc(T)
#endif//DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS

#ifdef DAMUFFIN_CONFIG_ALLOW_CLEANUP
    #include <damuffin/cleanup.h>
#endif//DAMUFFIN_CONFIG_ALLOW_CLEANUP

#endif//_DAMUFFIN_C_LIB_H_