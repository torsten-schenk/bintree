#include <string.h>
#include <stdio.h>
typedef struct mydata mydata_t;

#define BINTREE_PREFIX mydata
#define BINTREE_DATA mydata_t
#define BINTREE_FIELD node
#define BINTREE_USE_PARENT
#define BINTREE_USE_INDEX
#define BINTREE_USE_AVL
#define BINTREE_BZERO
#include "bintree-hdr.h"
struct mydata {
	mydata_node_t node;
	int value;
};
#include "bintree-impl.h"

static mydata_t *root = NULL;

static int cmp_value2(
		const mydata_t *a,
		const mydata_t *b)
{
	if(a->value < b->value)
		return -1;
	else if(a->value > b->value)
		return 1;
	else
		return 0;
}

static int cmp_value(
		const mydata_t *a,
		const void *b_)
{
	const mydata_t *b = b_;
	if(a->value < b->value)
		return -1;
	else if(a->value > b->value)
		return 1;
	else
		return 0;
}

static int cmp_field(
		const mydata_t *a,
		const void *b_)
{
	int b = *(const int*)b_;
	if(a->value < b)
		return -1;
	else if(a->value > b)
		return 1;
	else
		return 0;
}

static void insert(
		mydata_t *data)
{
	mydata_t *l;
	mydata_query(root, data, &l, NULL, cmp_value);
	mydata_insert(&root, l, data);
}

static int query(
		int value,
		mydata_t **l,
		mydata_t **u)
{
	return mydata_query(root, &value, l, u, cmp_field);
}

int main()
{
	mydata_t data[256];
	mydata_t *l;
	mydata_t *u;
	mydata_t *cur;
	for(size_t i = 0; i < sizeof(data) / sizeof(*data); i++)
		data[i].value = i % 16;
	for(size_t i = 0; i < 32; i++)
		insert(data + i);
	printf("SIZE: %zu\n", mydata_size(root));
	for(mydata_t *cur = mydata_first(root); cur; cur = mydata_next(cur)) {
		printf("CHECK: %p %d\n", cur, cur->value);
	}

	query(2, &l, &u);
	printf("IS: %p %p\n", l, u);

	cur = mydata_first(root);
	for(size_t i = 0; i < mydata_size(root); i++, cur = mydata_next(cur)) {
		printf("CHECK IDX: %zu %zu\n", i, mydata_index(cur));
	}

	for(size_t i = 0; i < mydata_size(root); i++) {
		cur = mydata_at(root, i);
		printf("CHECK GET: %zu %zu\n", i, mydata_index(cur));
	}




	printf("sort test\n");
	root = NULL;
	for(size_t i = 0; i < 32; i++)
		mydata_insert(&root, root, data + i);

	for(cur = mydata_first(root); cur; cur = mydata_next(cur)) 
		printf("%d\n", cur->value);

	mydata_sort(&root, cmp_value2);
	printf("SORTED:\n");
	for(cur = mydata_first(root); cur; cur = mydata_next(cur)) 
		printf("%d\n", cur->value);
	return 0;
}

