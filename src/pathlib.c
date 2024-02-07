#include <damuffin/pathlib.h>
#include <damuffin/config.h>

#undef DAMUFFIN_CONFIG_MAIN_SHORTHANDS
#undef DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS

#include <damuffin.h>

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const path_t pardir = "..";
static const path_t curdir = ".";
static const path_t altsep = "/";
static const path_t sep    = "\\";

static void __free_if_not_null(void* _Ptr) {
    if(_Ptr != NULL) {
        free(_Ptr);
    }
}

static path_t __replace(const char* _Str, const char* _Old, const char* _New) {
    char* result;
    i32 i, x = 0;

    i32 old_word_length = strlen(_Old); 
    i32 new_word_length = strlen(_New);
 
    // Counting the number of times old word occur in the string 
    for(i = 0; _Str[i] != '\0'; i++) { 
        if(strstr(&_Str[i], _Old) == &_Str[i]) { 
            x++; 
 
            // Jumping to index after the old word.
            i += old_word_length - 1; 
        } 
    } 
 
    // Making new string of enough length 
    result = Malloc(char, (i + x * (new_word_length - old_word_length) + 1));

    if(result == NULL) {
        return (path_t)NULL;
    }
 
    i = 0; 
    while(*_Str) { 
        // compare the substring with the result 
        if(strstr(_Str, _Old) == _Str) { 
            strcpy(&result[i], _New); 
            i += new_word_length;
            _Str += old_word_length;
        } else {
            result[i++] = *_Str++;
        }
    } 
 
    result[i] = '\0';
    return result;
}

// Converts all slashes to backslashes, e.g. A\B/C and A/B/C all become A\B\C (A\\B\\C).
static path_t __prep_path(path_t _Path) {
    size_t path_length = strlen(_Path);

    if(path_length < 1) {
        VarMalloc(p, char, 2);

        if(p == NULL) {
            return (path_t)NULL;
        }
        
        p[0] = '.';
        p[1] = '\0';
        
        return p;
    }

    // Could return NULL if malloc fails.
    return __replace(_Path, altsep, sep);
}

static path_t __join_path_t(path_t _Base, path_t _Tail) {
    size_t base_length = strlen(_Base);
    size_t tail_length = strlen(_Tail);
    size_t result_length = base_length + tail_length;

    VarMalloc(result, char, (result_length + 1));
    result[result_length] = '\0';

    strncpy(result,               _Base, base_length);
    strncpy(result + base_length, _Tail, tail_length);

    return result;
}

static void __add_path_prefix(path_t _Prefix, path_t _Path, size_t _PrefixSize) {
    for(size_t x = 0; x < _PrefixSize; x++) {
        _Path[x] = _Prefix[x];
    }
}

// Source: https://github.com/python/cpython/blob/01dceba13e872e9ca24b8e00a2b75db3d0d6c1a3/Lib/ntpath.py#L555
/* Normalize a path, e.g. A//B, A/./B and A/foo/../B all become A\B. 
 * Only returns NULL if a memory allocation fails. */
static path_t __norm_path(path_t _Path) {
    path_t path = __prep_path(_Path);

    if(path == (path_t)NULL) {
        return (path_t)NULL;
    }

    path_root_t detail = __split_root(path);
    path = detail.path;

    path_t prefix = __join_path_t(detail.drive, detail.root);

    size_t prev_length;
    size_t token_length;
    size_t buffer_length;

    size_t path_length = strlen(path);
    size_t prefix_length = strlen(prefix);
    size_t i = prefix_length;

    if(path_length <= 0) {
        if(strcmp(prefix, "") == 0) {
            return prefix;
        }

        VarMalloc(p, char, (prefix_length + 2));

        if(p == NULL) {
            return (path_t)NULL;
        }

        p[prefix_length + 0] = '.';
        p[prefix_length + 1] = '\0';
        
        __add_path_prefix(prefix, p, prefix_length);

        return p;
    }

    char* prev  = NULL;
    char* token = strtok(path, "\\");

    // (+2) for ending (\) and null terminator
    VarMalloc(buffer, char, (path_length + prefix_length + 2));
    
    if(buffer == NULL) {
        return (path_t)NULL;
    }

    buffer[path_length + 1] = '\0';

    do {
        if(strcmp(token, "..") == 0) {
            if(prev == NULL || strcmp(prev, ".") == 0 || strcmp(prev, "..") == 0) {
                // Copy null terminator as well.
                strncpy(buffer + i, "..\\", 4);
                i += 3;

                prev  = token;
                token = strtok(NULL, "\\");
            } else {
                // (+1) to account for (\)
                prev_length = strlen(prev) + 1;

                while(prev_length != 0) {
                    buffer[i] = 0;
                    prev_length--;
                    i--;
                }

                prev  = NULL;
                token = strtok(NULL, "\\");
            }
        } else if(strcmp(token, ".") == 0) {
            prev  = token;
            token = strtok(NULL, "\\");
        } else {
            token_length = strlen(token);

            strncpy(buffer + i, token, token_length);
            i += token_length;

            // Copy null terminator as well.
            strncpy(buffer + i, "\\", 2);
            i++;

            prev  = token;
            token = strtok(NULL, "\\");
        }
    } while(token != NULL);

    __add_path_prefix(prefix, buffer, prefix_length);

    if(strcmp(buffer, "") == 0) {
        buffer = (char*)realloc(buffer, sizeof(char) * 2);

        if(buffer == NULL) {
            return (path_t)NULL;
        }
        
        buffer[0] = '.';
        buffer[1] = '\0';

        return buffer;
    }

    // Remove the ending (\)
    buffer[--i] = '\0';
    buffer_length = strlen(buffer);

    if(buffer_length != (path_length + prefix_length)) {
        // (+1) for null terminator
        buffer = (char*)realloc(buffer, sizeof(char) * (buffer_length + 1));

        if(buffer == NULL) {
            return (path_t)NULL;
        }

        buffer[buffer_length] = '\0';
    }

    return buffer;
}

static path_t __get_current_dir() {
    VarMalloc(cwd, char, PATH_MAX);
    
    if(getcwd(cwd, PATH_MAX) != NULL) {
        return cwd;
    }

    return (path_t)NULL;
}

// Gets the drive of a path and returns NULL if none is found.
static path_t __get_path_drive(path_t _Path) {
    size_t path_length = strlen(_Path);

    if(path_length < 2) {
        return (path_t)NULL;
    }

    if((isalpha(_Path[0]) && _Path[0] != '_') && _Path[1] == ':') {
        VarMalloc(drive, char, 3);

        drive[0] = _Path[0];
        drive[1] = _Path[1];
        drive[2] = '\0';

        return drive;
    }

    return (path_t)NULL;
}

static path_t __lstrip_backslash(path_t _Str) {
    size_t size = strlen(_Str);

    if(!size) {
        return (path_t)NULL;
    }

    size_t i = 0;
    while(i < size && _Str[i] == '\\') {
        i++;
    }

    // (+1) for null terminator.
    VarMalloc(str, char, ((size - i) + 1));

    if(str == NULL) {
        return (path_t)NULL;
    }

    strcpy(str, _Str + i);

    return str;
}

static path_t __rstrip_backslash(path_t _Str) {
    size_t size = strlen(_Str);

    if(!size) {
        return (path_t)NULL;
    }

    // (+1) for null terminator.
    VarMalloc(str, char, (size + 1));

    if(str == NULL) {
        return (path_t)NULL;
    }

    strcpy(str, _Str);

    size_t i = size - 1;
    while(i > 0 && str[i] == '\\') {
        i--;
    }

    str[i + 1] = '\0';

    return str;
}

static path_temp_t __create_path_temp(path_t _Path) {
    path_temp_t __o = {
        .size = strlen(_Path),
        .path = _Path
    };

    return __o;
}

static char* __safe_access_index(path_temp_t _Path, size_t index) {
    if(index >= _Path.size) {
        return (char*)NULL;
    }

    VarMalloc(c, char, 2);
    c[0] = _Path.path[index];
    c[1] = '\0';
    return c;
}

static char* __safe_access_range(path_temp_t _Path, size_t start, size_t end) {
    if(_Path.size == 0) {
        return "\0";
    }

    if(start > end) {
        end = start;
    }
    
    if(end == (size_t)NULL) {
        end = _Path.size;
    } else if(end >= _Path.size) {
        end = _Path.size;
    }

    if(start == (size_t)NULL) {
        start = 0;
    } else if(start >= _Path.size) {
        start = _Path.size;
    }

    size_t range_length = end - start;
    // (+1) for null terminator
    VarMalloc(buffer, char, (range_length + 1));

    strncpy(buffer, _Path.path + start, range_length);
    buffer[range_length] = '\0';

    return buffer;
}

static find_result_t __find_backslash(path_temp_t _Str, size_t start) {
    find_result_t result = {
        .index = 0,
        .found = false
    };
    
    if(start >= _Str.size) {
        return result;
    }

    for(size_t i = _Str.size - 1; i >= start; i--) {
        if(_Str.path[i] == '\\') {
            result.index = i;
            result.found = true;
        }
    }

    return result;
}

static char* __toupper(char* _Str) {
    size_t str_length = strlen(_Str);

    // (+1) for null terminator.
    VarMalloc(str_copy, char, (str_length + 1));
    strcpy(str_copy, _Str);

    for(size_t i = 0; i < str_length; i++) {
        str_copy[i] = toupper(str_copy[i]);
    }

    return str_copy;
}

static bool __path_startswith(path_temp_t _Path, char* _Str, size_t start) {
    size_t str_length = strlen(_Str);
    
    if(str_length >= _Path.size) {
        return false;
    }

    if(start >= _Path.size) {
        return strcmp(_Str, "") == 0;
    }

    for(size_t i = 0; i < str_length; i++) {
        if(_Str[i] != _Path.path[start + i]) {
            return false;
        }
    }

    return true;
}

// Works the same as the python standard libraries splitroot function from ntpath
// Source: https://github.com/python/cpython/blob/01dceba13e872e9ca24b8e00a2b75db3d0d6c1a3/Lib/ntpath.py#L171
static path_root_t __split_root(path_t _AbsPath) {
    path_temp_t normp = __create_path_temp(__prep_path(_AbsPath));
    char* unc_prefix = "\\\\?\\UNC\\";

    #define _PATH_ROOT_T(v, d, r, p) path_root_t v = {.drive = d, .root = r, .path = p}

    if(normp.path == NULL) {
        _PATH_ROOT_T(null, NULL, NULL, NULL);

        return null;
    }

    if(strcmp(__safe_access_index(normp, 0), "\\") == 0) {
        if(strcmp(__safe_access_index(normp, 1), "\\") == 0) {
            // UNC drives, e.g. \\server\share or \\?\UNC\server\share
            // Device drives, e.g. \\.\device or \\?\device
            size_t start = strcmp(__toupper(__safe_access_range(normp, 0, 8)), unc_prefix) == 0 ? 8 : 2;
            find_result_t index = __find_backslash(normp, start);

            if(!index.found) {
                _PATH_ROOT_T(result, normp.path, "", "");
                return result;
            }

            find_result_t index2 = __find_backslash(normp, index.index + 1);
            if(!index2.found) {
                _PATH_ROOT_T(result, normp.path, "", "");
                return result;
            }

            char* drive = __safe_access_range(normp, 0, index2.index);
            char* root  = __safe_access_range(normp, index2.index, index2.index + 1);
            char* path  = __safe_access_range(normp, index2.index + 1, normp.size);

            _PATH_ROOT_T(result, drive, root, path);

            return result;
        } else {
            // Relative path with root, e.g. \Windows
            char* root = __safe_access_index(normp, 0);
            VarMalloc(path, char, normp.size);

            strncpy(path, normp.path + 1, normp.size);
            path[normp.size] = '\0';

            _PATH_ROOT_T(result, "", root, path);
            return result;
        }
    } else if(strcmp(__safe_access_index(normp, 1), ":") == 0) {
        if(strcmp(__safe_access_index(normp, 2), "\\") == 0) {
            char* drive = __safe_access_range(normp, 0, 2);
            char* root  = __safe_access_index(normp, 2);
            char* path  = __safe_access_range(normp, 3, normp.size);
            
            _PATH_ROOT_T(result, drive, root, path);

            return result;
        } else {
            char* drive = __safe_access_range(normp, 0, 2);
            char* path  = __safe_access_range(normp, 2, normp.size);

            _PATH_ROOT_T(result, drive, "", path);

            return result;
        }
    } else {
        _PATH_ROOT_T(result, "", "", normp.path);
        
        return result;
    }

    #undef _PATH_ROOT_T
}

// Source: https://github.com/python/cpython/blob/01dceba13e872e9ca24b8e00a2b75db3d0d6c1a3/Lib/ntpath.py#L80
bool is_path_absolute(path_t _Path) {
    path_temp_t temp = __create_path_temp(__prep_path(_Path));
    char* s = __safe_access_range(temp, 0, 3);

    return __path_startswith(temp, ":\\", 1) || __path_startswith(temp, "\\\\", 0);
}

bool is_path_relative(path_t _Path) {
    return !is_path_absolute(_Path);
}

path_split_t path_split(path_t _Path) {
    path_root_t detail = __split_root(_Path);
    path_temp_t p = __create_path_temp(_Path);

    size_t i = p.size;
    path_t x = __safe_access_index(p, i - 1);

    while(i != 0 && (strcmp(x, "\\") != 0 && strcmp(x, "/") != 0)) {
        i--;
        x = __safe_access_index(p, i - 1);
    }

    path_t head = __safe_access_range(p, 0, i);
    path_t tail = __safe_access_range(p, i, p.size);

    path_t prefix = __join_path_t(detail.drive, detail.root);

    // TODO rstrip (\) and (/)
    size_t prefix_length = strlen(prefix);

    if(strcmp(head, "\0") == 0) {
        head = prefix;
    } else {
        if(strcmp(prefix, "") != 0) {
            size_t head_length = strlen(head) + prefix_length;
            
            head = (char*)realloc(head, (head_length + 1));
            head[head_length] = '\0';

            __add_path_prefix(prefix, head, prefix_length);
        }
    }

    path_split_t result = {
        .head = head,
        .tail = tail
    };

    return result;
}

path_t path_absolute(path_t _RelPath) {
    if(is_path_absolute(_RelPath)) {
        return __norm_path(_RelPath);
    }

    path_t norm_path;

    char* base = __get_current_dir();
    char* path = path_join(base, _RelPath);

    path_root_t base_root = __split_root(base);
    path_root_t norm_root = __split_root(path);

    if(base == NULL || path == NULL) {
        __free_if_not_null(base);
        __free_if_not_null(path);

        return (path_t)NULL;
    }

    norm_path = __norm_path(path);
    base_root = __split_root(base);
    norm_root = __split_root(norm_path);

    if(strcmp(base_root.drive, norm_root.drive) != 0 || strcmp(base_root.root, norm_root.root) != 0) {
        char* drive = base_root.drive;
        char* root  = base_root.root;

        size_t drive_length = strlen(drive);
        size_t root_length  = strlen(root);
        size_t norm_length  = strlen(norm_path);

        size_t path_length = drive_length + root_length + norm_length;

        VarMalloc(path, char, (path_length + 1));
        path[path_length] = '\0';

        strncpy(path, drive, drive_length);
        strncpy(path + drive_length, root, root_length);
        strncpy(path + (drive_length + root_length), norm_path, norm_length);

        return path;
    }

    return norm_path;
}

path_t path_basename(path_t _Path) {
    path_split_t __o = path_split(_Path);
    __free_if_not_null(__o.head);
    return __o.tail;
}

path_t path_dirname(path_t _Path) {
    path_split_t __o = path_split(_Path);
    __free_if_not_null(__o.tail);
    return __o.head;
}

path_t path_join(path_t _Base, path_t _Tail) {
    char* base = __prep_path(_Base);
    char* tail = __prep_path(_Tail);

    if(base == NULL || tail == NULL) {
        __free_if_not_null(base);
        __free_if_not_null(tail);

        return (path_t)NULL;
    }

    char* _base = __rstrip_backslash(base);
    char* _tail = __lstrip_backslash(tail);

    __free_if_not_null(base);
    __free_if_not_null(tail);

    base = strcat(_base, "\\");
    return strcat(base, _tail);
}

bool path_exists(path_t _Path) {
    return is_path_file(_Path) || is_path_dir(_Path);
}

bool is_path_dir(path_t _Path) {
    struct stat s;
    stat(_Path, &s);
    return S_ISDIR(s.st_mode);
}

bool is_path_file(path_t _Path) {
    struct stat s;
    stat(_Path, &s);
    return S_ISREG(s.st_mode);
}