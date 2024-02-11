#include <damuffin/strlib.h>
#include <damuffin/config.h>

#undef DAMUFFIN_CONFIG_MAIN_SHORTHANDS
#undef DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS

#include <damuffin/dynamic/dynidx.h>
#include <damuffin.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

string_t str_null() {
    string_t null = {
        .length = 0,
        .buffer = (cstr_t)NULL
    };

    return null;
}


string_t str_join(string_t _Str1, string_t _Str2) {
    if(str_is_null(_Str1) || str_is_null(_Str2)) {
        return str_null();
    }

    size_t length = _Str1.length + _Str2.length;
    cstr_t buffer = cstr_alloc(length);

    if(buffer == NULL) {
        return str_null();
    }

    return str_from_cstr_s(buffer, length);
}


result_t str_find(string_t _Str, string_t _Word, size_t _Start, size_t _End) {
    string_t slice = str_slice(_Str, _Start, _End);
    result_t result = {
        .index = 0,
        .found = false
    };

    if(str_is_null(slice) || str_is_null(_Word) || _Word.length >= slice.length) {
        str_free_if_not_null(slice);
        
        return result;
    }

    cstr_t buffer = cstr_alloc(_Word.length);

    if(buffer == NULL) {
        str_free_if_not_null(slice);

        return result;
    }

    size_t s = 0;
    size_t e = _Word.length - 1;

    while(e != slice.length) {
        strncpy(buffer, slice.buffer + s, _Word.length);

        if(cstr_compare(buffer, _Word.buffer)) {
            result.index = _Start + s;
            result.found = true;
            break;
        }
        
        s++;
        e++;
    }

    str_free_if_not_null(slice);
    cstr_free_if_not_null(buffer);

    return result;
}

string_t str_index(string_t _Str, size_t _Index) {
    if(str_is_null(_Str) || _Index >= _Str.length) {
        return str_null();
    }

    return char_to_str(_Str.buffer[_Index]);
}

// Not inclusive of _End
string_t str_slice(string_t _Str, size_t _Start, size_t _End) {
    if(str_is_null(_Str) || _Str.length == 0) {
        return str_null();
    }

    if(_Start > _End) {
        _End = _Start;
    }
    
    if(_End > _Str.length) {
        _End = _Str.length;
    }

    if(_Start > _Str.length) {
        _Start = _Str.length;
    }

    size_t length = _End - _Start;
    cstr_t buffer = cstr_new_s(_Str.buffer + _Start, length);

    string_t slice = {
        .length = length,
        .buffer = buffer
    };

    return slice;
}

string_t str_lstrip(string_t _Str, string_t _Chars) {
    if(str_is_null(_Str) || str_is_null(_Chars)) {
        return str_null();
    }

    size_t i;

    for(i = 0; i < _Str.length; i++) {
        if(!char_in_str(_Str.buffer[i], _Chars)) {
            break;
        }
    }    

    size_t length = _Str.length - i;
    cstr_t buffer = cstr_alloc(length);

    if(buffer == NULL) {
        return str_null();
    }

    strncpy(buffer, _Str.buffer + i, length);
    return str_from_cstr_s(buffer, length);
}

string_t str_rstrip(string_t _Str, string_t _Chars) {
    if(str_is_null(_Str) || str_is_null(_Chars)) {
        return str_null();
    }

    size_t i;

    for(i = _Str.length - 1; i != 0; i--) {
        if(!char_in_str(_Str.buffer[i], _Chars)) {
            break;
        }
    }

    size_t length = i + 1;
    cstr_t buffer = cstr_alloc(length);

    if(buffer == NULL) {
        return str_null();
    }

    strncpy(buffer, _Str.buffer, length);
    return str_from_cstr_s(buffer, length);
}


string_t str_append(string_t _Str, string_t _New) {
    if(str_is_null(_Str) || str_is_null(_New)) {
        return str_null();
    }

    size_t length = _Str.length + _New.length;
    cstr_t buffer = cstr_alloc(length);

    if(buffer == NULL) {
        return str_null();
    }

    strncpy(buffer,               _Str.buffer, _Str.length);
    strncpy(buffer + _Str.length, _New.buffer, _New.length);

    return str_from_cstr_s(buffer, length);
}

string_t str_prepend(string_t _Str, string_t _New) {
    if(str_is_null(_Str) || str_is_null(_New)) {
        return str_null();
    }

    size_t length = _Str.length + _New.length;
    cstr_t buffer = cstr_alloc(length);

    if(buffer == NULL) {
        return str_null();
    }

    strncpy(buffer,               _New.buffer, _New.length);
    strncpy(buffer + _New.length, _Str.buffer, _Str.length);

    return str_from_cstr_s(buffer, length);
}


string_t str_toupper(string_t _Str) {
    string_t copy = str_new_s(_Str.buffer, _Str.length);

    if(str_is_null(copy)) {
        return str_null();
    }

    for(size_t i = 0; i < _Str.length; i++) {
        copy.buffer[i] = toupper(copy.buffer[i]);
    }

    return copy;
}

string_t str_tolower(string_t _Str) {
    string_t copy = str_new_s(_Str.buffer, _Str.length);

    if(str_is_null(copy)) {
        return str_null();
    }

    for(size_t i = 0; i < _Str.length; i++) {
        copy.buffer[i] = tolower(copy.buffer[i]);
    }

    return copy;
}

string_t str_replace(string_t _Str, string_t _Old, string_t _New, size_t _Max) {
    if(str_is_null(_Str) || str_is_null(_Old) || str_is_null(_New)) {
        return str_null();
    }

    if(_Max <= 0) {
        return str_new_s(cstr_new_s(_Str.buffer, _Str.length), _Str.length);
    }

    dynidx_t indexes = dynidx_new();
    result_t window = str_find(_Str, _Old, 0, _Str.length);

    size_t count = 0;
    while(window.found && (count != _Max)) {
        dynidx_push(&indexes, window.index);

        window = str_find(_Str, _Old, window.index + _Old.length - 1, _Str.length);
        count++;
    }

    if(count == 0) {
        return str_new_s(cstr_new_s(_Str.buffer, _Str.length), _Str.length);
    }

    size_t length = _Str.length + count * (_New.length - _Old.length);
    cstr_t buffer = cstr_alloc(length);

    size_t idx;
    size_t strdst;
    size_t bufptr = 0;
    size_t strptr = 0;

    for (size_t i = 0; i < indexes.endptr; i++) {
        idx = indexes.block[i];
        strdst = idx - strptr;

        strncpy(buffer + bufptr, _Str.buffer + strptr, strdst);

        bufptr += strdst;
        strptr =  idx;

        strncpy(buffer + bufptr, _New.buffer, _New.length);

        bufptr += _New.length;
        strptr += _Old.length;
    }

    strncpy(buffer + bufptr, _Str.buffer + strptr, _Str.length - strptr);

    dynidx_free(&indexes);

    return str_new_s(buffer, length);
}

string_t str_replace_all(string_t _Str, string_t _Old, string_t _New) {

}

bool str_is_null(string_t _Str) {
    return _Str.buffer == (cstr_t)NULL;
}

bool str_compare(string_t _Str1, string_t _Str2) {
    return cstr_compare(_Str1.buffer, _Str2.buffer);
}

bool str_startswith(string_t _Str, string_t _Prefix, size_t _Start, size_t _End) {
    string_t slice = str_slice(_Str, _Start, _End);

    if(_Start >= slice.length) {
        return str_compare(_Prefix, slice);
    }

    if(_Prefix.length > slice.length) {
        return false;
    }

    if(str_is_null(slice)) {
        return false;
    }

    for(size_t i = 0; i < _Prefix.length; i++) {
        if(_Prefix.buffer[i] != slice.buffer[i]) {
            return false;
        }
    }

    str_free_if_not_null(slice);

    return true;
}

bool str_endswith(string_t _Str, string_t _Suffix, size_t _Start, size_t _End) {
    string_t slice = str_slice(_Str, _Start, _End);

    if(_Start >= slice.length) {
        return str_compare(_Suffix, slice);
    }

    if(_Suffix.length > slice.length) {
        return false;
    }

    if(str_is_null(slice)) {
        return false;
    }

    size_t x = slice.length - 1;
    for(size_t i = _Suffix.length - 1; i != 0; i--) {
        if(_Suffix.buffer[i] != slice.buffer[x]) {
            return false;
        }

        x--;
    }

    str_free_if_not_null(slice);

    return true;
}

bool char_in_str(char _Char, string_t _Str) {
    for(size_t i = 0; i < _Str.length; i++) {
        if(_Str.buffer[i] == _Char) {
            return true;
        }
    }

    return false;
}

bool char_in_cstr(char _Char, cstr_t _Str) {
    string_t str = str_from_cstr(_Str);
    bool result = char_in_str(_Char, str);
    str_free_if_not_null(str);
    return result;
}

/*
cstr_t cstr_join(char*, char*);
cstr_t cstr_find(const char*, size_t, size_t);
cstr_t cstr_index(string_t, size_t);
cstr_t cstr_slice(string_t, size_t, size_t);
*/

cstr_t cstr_lstrip(char* _Str, const char* _Chars) {
    string_t __s = str_new(_Str);
    string_t __c = str_new(_Chars);
    string_t __o = str_lstrip(__s, __c);

    str_free_if_not_null(__s);
    str_free_if_not_null(__c);

    return __o.buffer;
}

cstr_t cstr_rstrip(char* _Str, const char* _Chars) {
    string_t __s = str_new(_Str);
    string_t __c = str_new(_Chars);
    string_t __o = str_rstrip(__s, __c);

    str_free_if_not_null(__s);
    str_free_if_not_null(__c);

    return __o.buffer;
}

/*
cstr_t cstr_append(const char*);
cstr_t cstr_prepend(const char*);
cstr_t cstr_toupper(const char*);
cstr_t cstr_tolower(const char*);
cstr_t cstr_replace(const char*, const char*, const char*);
*/

bool cstr_compare(const char* _Str1, const char* _Str2) {
    return strcmp(_Str1, _Str2) == 0;
}

bool cstr_startswith(const char* _Str, const char* _Prefix, size_t _Start, size_t _End) {
    string_t __s = str_new(_Str);
    string_t __p = str_new(_Prefix);
    
    bool __o = str_startswith(__s, __p, _Start, _End);
    
    str_free_if_not_null(__s);
    str_free_if_not_null(__p);
    
    return __o;
}

bool cstr_endswith(const char* _Str, const char* _Suffix, size_t _Start, size_t _End) {
    string_t __s = str_new(_Str);
    string_t __p = str_new(_Suffix);

    bool __o = str_endswith(__s, __p, _Start, _End);
    
    str_free_if_not_null(__s);
    str_free_if_not_null(__p);
    
    return __o;
}

string_t char_to_str(char _C) {
    size_t length = 1;
    cstr_t buffer = char_to_cstr(_C);

    string_t __o = {
        .length = length,
        .buffer = buffer
    };

    return __o;
}

cstr_t char_to_cstr(char _C) {
    VarMalloc(__o, char, 2);
    
    if(__o == NULL) {
        return (cstr_t)NULL;
    }

    __o[0] = _C;
    __o[1] = '\0';
    
    return __o;
}

cstr_t cstr_new(const char* _Str) {
    size_t _Size = strlen(_Str);
    cstr_t _CStr = cstr_alloc(_Size);
    
    if(_CStr == NULL) {
        return (cstr_t)NULL;
    }
    
    strncpy(_CStr, _Str, _Size);

    return _CStr;
}

cstr_t cstr_new_s(const char* _Str, size_t _Size) {
    cstr_t _CStr = cstr_alloc(_Size);
    
    if(_CStr == NULL) {
        return (cstr_t)NULL;
    }

    strncpy(_CStr, _Str, _Size);

    return _CStr;
}

string_t str_new(const char* _Str) {
    size_t length = strlen(_Str);
    cstr_t buffer = cstr_new_s(_Str, length);

    string_t __o = {
        .length = length,
        .buffer = buffer
    };

    return __o;
}

string_t str_new_s(const char* _Str, size_t _Size) {
    cstr_t buffer = cstr_new_s(_Str, _Size);

    string_t __o = {
        .length = _Size,
        .buffer = buffer
    };

    return __o;
}

string_t str_from_cstr(cstr_t _CStr) {
    size_t length = strlen(_CStr);
    cstr_t buffer = _CStr;

    string_t __o = {
        .length = length,
        .buffer = buffer
    };

    return __o;
}

string_t str_from_cstr_s(cstr_t _CStr, size_t _Length) {
    size_t length = _Length;
    cstr_t buffer = _CStr;

    string_t __o = {
        .length = length,
        .buffer = buffer
    };

    return __o;
}

cstr_t cstr_alloc(size_t _Size) {
    VarMalloc(__o, char, (_Size + 1));
    
    if(__o == NULL) {
        return (cstr_t)NULL;
    }
    
    __o[_Size] = '\0';

    return __o;
}

void str_free(string_t _Str) {
    cstr_free(_Str.buffer);
}

void cstr_free(cstr_t _CStr) {
    free(_CStr);
}

void str_free_if_not_null(string_t _Str) {
    cstr_free_if_not_null(_Str.buffer);
}

void cstr_free_if_not_null(cstr_t _CStr) {
    if(_CStr != (cstr_t)NULL) {
        cstr_free(_CStr);
    }
}