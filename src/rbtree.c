#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef max
#define max(a, b) ((a) < (b) ? (b) : (a))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef NDEBUG
#	define DBG(fmt, ...) (printf("[rbtree.c:%d]:%s :: " fmt "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__))
#	define DBG_ENTER DBG("entered");
#	define DBG_EXIT DBG("exited");
#else
#	define DBG_(fmt, ...)
#	define DBG_ENTER
#	define DBG_EXIT
#endif

struct _rbnode
{
	_rbnode *left, *right, *parent;
	int color : 1;
	int hasdata : 1;
};

_rbnode * _rbnode_new(void * (*malloc) (size_t), size_t elsize)
{
	return malloc(sizeof(_rbnode) + elsize);
}

void * _rbnode_elem(_rbnode * node)
{
	return ((char *)node) + sizeof(_rbnode);
}

void * _pool_new(size_t cap, size_t elsize)
{
	size_t actual_elsize = max(elsize, sizeof(void *));
	size_t size = cap * actual_elsize;
	size += sizeof(void *);
	void * pool = malloc(size);
	(void)memset(pool, 0, size);
	return pool;
}

struct _flheader
{
	
};

void * _pool_grow(void * pool, size_t oldcap, size_t newcap, size_t elsize)
{
	size_t actual_elsize = max(elsize, sizeof(int));
	size_t oldsize = oldcap * actual_elsize;
	size_t newsize = newcap * actual_elsize;
	void *newpool = realloc(pool, newsize);
	(void)memset(((char*)newpool) + oldsize, 0, newsize - oldsize);
	return newpool;
}

void * _pool_get(void * pool, size_t elsize, int index)
{
	return (void *)((char *)pool + elsize * index);
}

void * _freelist_insert(void * pool, size_t cap, size_t elsize)
{
	void * flptr = (void *)(*(size_t *)pool);
	if (!flptr) // free list is empty
	{
		// insert to the 
	}
	else
	{

	}
}

void test_pool()
{
	DBG_ENTER
	void * pool = _pool_new(5, sizeof(int));
	int * ipool = (int*)pool;
	DBG("pool : %p", pool);
	for (int i = 0; i < 5; i++)
	{
		DBG("    # %d : %d", i, *(int *)_pool_get(pool, sizeof(int), i));
	}
	for (int i = 0; i < 5; i++)
	{
		ipool[i] = i;
	}
	DBG("pool grown");
	pool = _pool_grow(pool, 5, 8, sizeof(int));
	ipool = (int*)pool;
	for (int i = 0; i < 8; i++)
	{
		DBG("    # %d : %d", i, *(int *)_pool_get(pool, sizeof(int), i));
	}

	DBG_EXIT
}

_rbnode * _insert_node_bst(_rbnode * root, const void * el, rb_CompareFunc compare, void * udata)
{
	_rbnode * parent = NULL;
	_rbnode * node = root;
	while (node)
	{
		parent = node;
		int cmpres = compare(el, parent, udata);
		if (cmpres < 0)
			node = parent->left;
		else if (cmpres > 0)
			node = parent->right;
		else
			return parent;
	}
	int cmpres = compare(el, parent, udata);
	if (cmpres < 0)
		node = parent->left;
	else if (cmpres > 0)
		node = parent->right;
	else
		return parent;
}

#define RBTREE_MINCAP 16

struct _rbtree
{
	_rbnode *root;
	size_t count, capacity;
	size_t elsize;
	rb_CompareFunc compare;
};

_rbtree * rbtree_new(size_t elsize, size_t cap, rb_CompareFunc compare, void * udata)
{
	DBG_ENTER
	// allocate _rbtree
	_rbtree * tree = malloc(sizeof(_rbtree));
	tree->count = 0;
	tree->capacity = cap ? cap : RBTREE_MINCAP;
	tree->elsize = elsize;
	tree->compare = compare;
	DBG("allocated _rbtree");
	// allocate pool with capacity
	DBG("sizeof(_rbnode) : %lu", sizeof(_rbnode));
	tree->root = _pool_new(cap, sizeof(_rbnode) + elsize);

	DBG_EXIT
	return tree;
}

const void * rbtree_insert(_rbtree * tree, const void * el)
{
	DBG_ENTER

	DBG_EXIT
}


const void * rbtree_get(_rbtree * tree, const void * el)
{
	DBG_ENTER

	DBG_EXIT
}
