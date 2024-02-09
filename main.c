//#include <damuffin/config.h>

/* Disabling certain configurations requires you to undefine any of the following macros */
//#undef DAMUFFIN_CONFIG_TYPE_SHORTHANDS
//#undef DAMUFFIN_CONFIG_MAIN_SHORTHANDS
//#undef DAMUFFIN_CONFIG_TYPE_DIRECTIVES
//#undef DAMUFFIN_CONFIG_STDLIB_MALLOC_SHORTHANDS
//#undef DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS
//#undef DAMUFFIN_CONFIG_ALLOW_INT128
//#undef DAMUFFIN_CONFIG_ALLOW_CLEANUP

#include <damuffin/strlib.h>
#include <damuffin.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
'MAIN(argc, argv)' translates to
int main(int argc, char* argv[])

'EMPTY_MAIN(0)' translates to
int main() { return 0; }

'DEFAULT_MAIN' translates to
int main(int argc, char* argv[])

USAGE EXAMPLES:

MAIN(count, args) {
    // count is int argc
    // args is char* argv[]
    return 0;
}

// Exits with exit code 1.
EMPTY_MAIN(1)

DEFAULT_MAIN {
    // int   argc   is defined
    // char* argv[] is defined

    return 0;
}
*/

/*
typedef struct MyStruct { ... } MyStruct_t;

'Malloc(MyStruct_t, 20)' translates to
(MyStruct_t*)malloc(sizeof(MyStruct_t) * 20)

'SingleMalloc(MyStruct_t)' tranlsates to
(MyStruct_t*)malloc(sizeof(MyStruct_t))

'VarMalloc(myVar, MyStruct_t, 20)' translates to
MyStruct_t* myVar = (MyStruct_t*)malloc(sizeof(MyStruct_t) * 20)

'SingleVarMalloc(myVar, MyStruct_t)' translates to
MyStruct_t* myVar = (MyStruct_t*)malloc(sizeof(MyStruct_t))

// You must end each macro with a `;` 
*/

DEFAULT_MAIN {
    string_t r = str_new("This is a sentence containing is. My is is not your is, y'know?");
    string_t p = str_new("is");
    string_t y = str_new("JESUS IS KING");
    string_t x = str_replace(r, p, y, 10);

    printf("%s\n", x.buffer);

    str_free(r);
    str_free(y);
    str_free(x);
    str_free(p);

    return 0;
}