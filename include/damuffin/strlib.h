#pragma once
#ifndef _DAMUFFIN_C_LIB_STRLIB_H_
#define _DAMUFFIN_C_LIB_STRLIB_H_ 1

#ifndef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #define DAMUFFIN_TEMPORARY_CONFIG__STRLIB
#endif

#include <damuffin/types.h>
#include <stdbool.h>

// cstr_t is a type that references char* made on the heap.
_T(char*) cstr_t;

_T(struct) _cstring {
    size_t length;
    cstr_t buffer;
} string_t;

_T(struct) _search_result {
    size_t index;
    bool   found;
} result_t;

string_t str_null();
string_t str_join(string_t, string_t);
result_t str_find(string_t, string_t, size_t, size_t);
string_t str_index(string_t, size_t);
string_t str_slice(string_t, size_t, size_t);
string_t str_lstrip(string_t, string_t);
string_t str_rstrip(string_t, string_t);
string_t str_append(string_t, string_t);
string_t str_prepend(string_t, string_t);
string_t str_toupper(string_t);
string_t str_tolower(string_t);
string_t str_replace(string_t, string_t, string_t, size_t);
string_t str_replace_all(string_t, string_t, string_t);

bool str_compare(string_t, string_t);
bool str_startswith(string_t, string_t, size_t, size_t);
bool str_endswith(string_t, string_t, size_t, size_t);
bool str_is_null(string_t);

cstr_t cstr_join(char*, char*);
cstr_t cstr_find(const char*, const char*, size_t, size_t);
cstr_t cstr_index(string_t, size_t);
cstr_t cstr_slice(string_t, size_t, size_t);
cstr_t cstr_lstrip(char*, const char*);
cstr_t cstr_rstrip(char*, const char*);
cstr_t cstr_append(const char*);
cstr_t cstr_prepend(const char*);
cstr_t cstr_toupper(const char*);
cstr_t cstr_tolower(const char*);
cstr_t cstr_replace(const char*, const char*, const char*);

bool cstr_compare(const char*, const char*);
bool cstr_startswith(const char*, const char*, size_t, size_t);
bool cstr_endswith(const char*, const char*, size_t, size_t);

bool     char_in_str(char, string_t);
bool     char_in_cstr(char, cstr_t);

string_t char_to_str(char);
cstr_t   char_to_cstr(char);

string_t str_new(const char*);
string_t str_new_s(const char*, size_t);

string_t str_from_cstr(cstr_t);
string_t str_from_cstr_s(cstr_t, size_t);

cstr_t cstr_new(const char*);
cstr_t cstr_new_s(const char*, size_t);

// Null terminated char*
cstr_t cstr_alloc(size_t);

void str_free(string_t);
void cstr_free(cstr_t);

void str_free_if_not_null(string_t);
void cstr_free_if_not_null(cstr_t);


#ifdef DAMUFFIN_TEMPORARY_CONFIG__STRLIB
    #undef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
    #undef DAMUFFIN_TEMPORARY_CONFIG__STRLIB
#endif

#endif//_DAMUFFIN_C_LIB_STRLIB_H_