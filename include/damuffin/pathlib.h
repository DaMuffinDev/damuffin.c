#pragma once
#ifndef _DAMUFFIN_C_LIB_PATHLIB_H_
#define _DAMUFFIN_C_LIB_PATHLIB_H_ 1

#ifndef _WIN32
    #error "<damuffin/pathlib.h> is only supported on Windows."
#endif

#ifndef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_TEMPORARY_CONFIG__PATHLIB
#endif

#include <damuffin/types.h>
#include <stdbool.h>
#include <stddef.h>

_T(char*) path_t;

_T(struct) __path_root {
    path_t drive;
    path_t root;
    path_t path;
} path_root_t;

_T(struct) __path_temp {
    size_t size;
    path_t path;
} path_temp_t;

_T(struct) __path_split {
    path_t head;
    path_t tail;
} path_split_t;

_T(struct) __find_result {
    size_t index;
    bool   found;
} find_result_t;

static path_temp_t __create_path_temp(path_t);
static char* __safe_access_index(path_temp_t, size_t);
static char* __safe_access_range(path_temp_t, size_t, size_t);
static find_result_t __find_backslash(path_temp_t _Str, size_t start);

static path_t __replace(const char*, const char*, const char*);
static path_t __prep_path(path_t);
static path_t __norm_path(path_t);
static path_t __get_current_dir();
static path_t __get_path_drive(path_t);
static path_t __lstrip_backslash(path_t);
static path_t __rstrip_backslash(path_t);
static path_root_t __split_root(path_t);

bool is_path_absolute(path_t);
bool is_path_relative(path_t);

path_split_t path_split(path_t);

path_t path_absolute(path_t);
path_t path_basename(path_t);
path_t path_dirname(path_t);
path_t path_join(path_t, path_t);

bool path_exists(path_t);
bool is_path_dir(path_t);
bool is_path_file(path_t);

#ifdef DAMUFFIN_TEMPORARY_CONFIG__PATHLIB
    #undef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #undef DAMUFFIN_TEMPORARY_CONFIG__PATHLIB
#endif

#endif//_DAMUFFIN_C_LIB_PATHLIB_H_