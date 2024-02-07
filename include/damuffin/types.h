#pragma once
#ifndef _DAMUFFIN_C_LIB_TYPES_H_
#define _DAMUFFIN_C_LIB_TYPES_H_ 1

#include <damuffin/util.h>

#ifdef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    _T(S char)  i8;
    _T(S short) i16;
    _T(S int)   i32;
    _T(S L L)   i64;

    _T(U char)  u8;
    _T(U short) u16;
    _T(U int)   u32;
    _T(U L L)   u64;

    _T(float)   f32;
    _T(double)  f64;

    #ifdef DAMUFFIN_CONFIG_TYPE_DIRECTIVES
        #define i8  i8
        #define i16 i16
        #define i32 i32
        #define i64 i64

        #define u8  u8
        #define u16 u16
        #define u32 u32
        #define u64 u64

        #define f32 f32
        #define f64 f64
    #endif//DAMUFFIN_CONFIG_TYPE_DIRECTIVES

    #ifdef DAMUFFIN_CONFIG_ALLOW_INT128
        _T(S int)    i128 _A(mode(TI));
        _T(U int)    u128 _A(mode(TI));
        _T(L double) f128;

        #ifdef DAMUFFIN_CONFIG_TYPE_DIRECTIVES
            #define i128 i128
            #define u128 u128
            #define f128 f128
        #endif//DAMUFFIN_CONFIG_TYPE_DIRECTIVES
    #endif//DAMUFFIN_CONFIG_ALLOW_INT128
#endif//DAMUFFIN_CONFIG_TYPE_SHORTHANDS

#endif//_DAMUFFIN_C_LIB_TYPES_H_