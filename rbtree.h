#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct _rbtree _rbtree;
typedef struct _rbnode _rbnode;

typedef _rbtree *rbtree;
typedef _rbnode *rbnode;

typedef int (*rb_CompareFunc) (const void * a, const void * b, void * udata); // returns (a < b) for increasing order, and (a > b) for decreasing order

rbtree rbtree_new(size_t elsize, size_t cap, rb_CompareFunc compare, void * udata);
// rbtree rbtree_new_with_allocator(void * (*malloc) (size_t), void * (*realloc) (void *, size_t), void (*free) (void *), size_t elsize, size_t cap, rb_CompareFunc compare, void * udata);

const void * rbtree_insert(rbtree tree, const void * el);
const void * rbtree_get(rbtree tree, const void * el);
