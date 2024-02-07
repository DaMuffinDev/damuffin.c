#pragma once
#ifndef _DAMUFFIN_C_LIB_CLEANUP_H_
#define _DAMUFFIN_C_LIB_CLEANUP_H_ 1

#undef _R
#undef _A
#undef _T

#undef L
#undef S
#undef U

#ifdef _DAMUFFIN_C_LIB_BROWSER_H_
    #undef _B_HELPER_DETAIL_FACTORY
    #undef _B_HELPER_DEFAULT
    #undef _B_HELPER_PROFILE
    #undef _B
#endif//_DAMUFFIN_C_LIB_BROWSER_H_

#endif//_DAMUFFIN_C_LIB_CLEANUP_H_