#include "../rbtree.h"
#include <stdio.h>

struct MyData
{
	const char* s; int i; float f;
};

int compare_mydata(const void * a, const void * b, void * udata)
{
	const struct MyData * mda = a;
	const struct MyData * mdb = b;
	return (mda->i) < (mdb->i);
}

int main()
{
	rbtree tree = rbtree_new(sizeof(struct MyData), 0, compare_mydata, NULL);
	rbtree_insert(tree, &(struct MyData){ .s="MyData #1", .i=3, .f=3.1415925 });

	return 0;
}
