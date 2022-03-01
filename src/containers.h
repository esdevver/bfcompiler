
#ifndef BFC_SRC_CONTAINERS_H_
#define BFC_SRC_CONTAINERS_H_

#include "stdlib.h"

typedef struct vector {
  void **start;
  size_t length;
  size_t capacity;
} vector;

void vector_init(vector *v);
void vector_reserve(vector *v, size_t capacity);
void vector_shink_to_fit(vector *v);
void vector_push(vector *v, void *item);
void *vector_get(vector *v, size_t index);
void *vector_last(vector *v);

#endif
