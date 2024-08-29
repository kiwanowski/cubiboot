#include "pool.h"

// impl from https://stackoverflow.com/a/11751232/652487

void pool_init(pool_t *p, size_t size) {
    p->next = (uint8_t*)&p[1]; // start after the pool_t struct
    p->end = p->next + size;
    return;
}

// void pool_reset(pool_t *p) {
//     // ???
// }

size_t pool_available(pool_t *p) {
    return p->end - p->next;
}

void * pool_alloc(pool_t *p, size_t size) {
    if( pool_available(p) < size ) return NULL;
    void *mem = (void*)p->next;
    p->next += size;
    return mem;
}
