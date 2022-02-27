
#include "containers.h"

void vector_init(vector *v) {
  v->length = 0;
  v->capacity = 0;
  v->start = NULL;
}

void vector_reserve(vector *v, size_t capacity) {
  if (capacity >= v->length) {
    if (v->start != NULL) {
      v->start = realloc(v->start, sizeof(void *) * capacity);
    } else {
      v->start = malloc(sizeof(void *) * capacity);
    }
    v->capacity = capacity;
  }
}

void vector_push(vector *v, void *item) {
  size_t wanted_capacity;
  if (v->length == v->capacity) {
    if (v->capacity == 0)
      wanted_capacity = 4;
    else
      wanted_capacity = v->capacity * 2;
    vector_reserve(v, wanted_capacity);
  }
  v->start[v->length++] = item;
}

void *vector_get(vector *v, size_t index) { return v->start[index]; }

void *vector_last(vector *v) {
  if (v->length == 0)
    return NULL;
  else
    return vector_get(v, v->length - 1);
}
