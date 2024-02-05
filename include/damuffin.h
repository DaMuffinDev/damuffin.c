#pragma once
#ifndef _DAMUFFIN_C_LIB_H_
#define _DAMUFFIN_C_LIB_H_ 1

#include <damuffin/config.h>
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

    #ifdef DAMUFFIN_CONFIG_ALLOW_INT128
        _T(S int)    i128 _A(mode(TI));
        _T(U int)    u128 _A(mode(TI));
        _T(L double) f128;
    #endif//DAMUFFIN_CONFIG_ALLOW_INT128
#endif//DAMUFFIN_CONFIG_TYPE_SHORTHANDS

#ifdef DAMUFFIN_CONFIG_ALLOW_CLEANUP
    #include <damuffin/cleanup.h>
#endif//DAMUFFIN_CONFIG_ALLOW_CLEANUP

#endif//_DAMUFFIN_C_LIB_H_