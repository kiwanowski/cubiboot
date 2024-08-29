#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t* next;
  uint8_t* end;
} pool_t;


void pool_init(pool_t *p, size_t size);
// void pool_reset(pool_t *p);
size_t pool_available(pool_t *p);
void *pool_alloc(pool_t *p, size_t size);
