#include <damuffin/dynamic/dynidx.h>
#include <damuffin/config.h>

#undef DAMUFFIN_CONFIG_MAIN_SHORTHANDS
#undef DAMUFFIN_CONFIG_STDLIB_CALLOC_SHORTHANDS

#include <damuffin.h>
#include <stdlib.h>
#include <mem.h>

dynidx_t dynidx_null() {
    dynidx_t null = {
        .nxtsize = 0,
        .memsize = 0,
        .endptr  = 0,
        .block   = (size_t*)NULL
    };

    return null;
}

bool dynidx_is_null(dynidx_t _Mem) {
    return _Mem.block == (size_t*)NULL;
}

dynidx_t dynidx_new() {
    dynidx_t __o = {
        .nxtsize = 8,
        .memsize = 4,
        .endptr  = 0,
        .block   = Malloc(size_t, 4)
    };

    if(dynidx_is_null(__o)) {
        return dynidx_null();
    }

    return __o;
}

void dynidx_free(dynidx_t* _Mem) {
    free(_Mem->block);
}

dynidx_t dynidx_push(dynidx_t* _Mem, size_t _Index) {
    __dynidx_reserve(_Mem);

    if(dynidx_is_null(*_Mem)) {
        return dynidx_null();
    }

    _Mem->block[_Mem->endptr] = _Index;
    _Mem->endptr++;

    return *_Mem;
}

static void __dynidx_reserve(dynidx_t* _Mem) {
    if(_Mem->endptr + 1 == _Mem->memsize) {
        _Mem->block = (size_t*)realloc(_Mem->block, sizeof(size_t) * _Mem->nxtsize);
        _Mem->memsize = _Mem->nxtsize;
        _Mem->nxtsize += 4;
    }
}