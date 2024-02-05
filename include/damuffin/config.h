#pragma once
#ifndef _DAMUFFIN_C_LIB_CONFIG_H_
#define _DAMUFFIN_C_LIB_CONFIG_H_ 1

/* Enable type shorthands for int, uint, and float */
#define DAMUFFIN_CONFIG_TYPE_SHORTHANDS

/* Enable macros for each type to check if it is defined using a macro */
#define DAMUFFIN_CONFIG_TYPE_DIRECTIVES

/* Allow the use of 128-bit integer types (i128, u128, f128) */
#define DAMUFFIN_CONFIG_ALLOW_INT128

/* Add cleanup import to #undef utility macros */
#define DAMUFFIN_CONFIG_ALLOW_CLEANUP

#endif//_DAMUFFIN_C_LIB_CONFIG_H_