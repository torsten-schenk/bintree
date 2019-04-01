#include <string.h>
#include <stdio.h>
typedef struct mydata mydata_t;
typedef struct mytwo mytwo_t;
typedef struct mymulti mymulti_t;

#define MYDATA_PREFIX mydata
#define MYDATA_FN static inline
#define MYDATA_DATA mydata_t
#define MYDATA_FIELD node
#define MYDATA_USE_PARENT 1
#define MYDATA_USE_INDEX 1
#define MYDATA_USE_AVL 1
#define MYDATA_USE_BZERO 1
#define MYDATA_INDEX size_t

#define MYASCEND_PREFIX myascend
#define MYASCEND_FN static inline
#define MYASCEND_DATA mytwo_t
#define MYASCEND_FIELD ascend
#define MYASCEND_USE_PARENT 1
#define MYASCEND_USE_INDEX 1
#define MYASCEND_USE_AVL 1
#define MYASCEND_USE_BZERO 1
#define MYASCEND_INDEX size_t

#define MYDESCEND_PREFIX mydescend
#define MYDESCEND_FN static inline
#define MYDESCEND_DATA mytwo_t
#define MYDESCEND_FIELD descend
#define MYDESCEND_USE_PARENT 1
#define MYDESCEND_USE_INDEX 1
#define MYDESCEND_USE_AVL 1
#define MYDESCEND_USE_BZERO 1
#define MYDESCEND_INDEX size_t

#define MYMULTI_PREFIX mymulti
#define MYMULTI_FIELD fields
#define MYMULTI_FN static inline
#define MYMULTI_DATA mymulti_t
#define MYMULTI_USE_PARENT 1
#define MYMULTI_USE_INDEX 1
#define MYMULTI_USE_AVL 1
#define MYMULTI_USE_BZERO 1
#define MYMULTI_USE_MULTI 1
#define MYMULTI_INDEX size_t
#define MYMULTI_MULTI int

struct mydata {
#define BINTREE_CONFIG MYDATA
#include "bintree-hdr.h"
	int value;
};

struct mytwo {
#define BINTREE_CONFIG MYASCEND
#include "bintree-hdr.h"
#define BINTREE_CONFIG MYDESCEND
#include "bintree-hdr.h"
	int value;
};

struct mymulti_field {
#define BINTREE_CONFIG MYMULTI
#include "bintree-hdr.h"
};

struct mymulti {
	struct mymulti_field fields[10];
};


#define BINTREE_CONFIG MYDATA
#include "bintree-impl.h"
#define BINTREE_CONFIG MYASCEND
#include "bintree-impl.h"
#define BINTREE_CONFIG MYDESCEND
#include "bintree-impl.h"
#define BINTREE_CONFIG MYMULTI
#include "bintree-impl.h"

static mydata_t *root = NULL;

static int cmp_value(
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

static int cmp_ascend(
		const mytwo_t *a,
		const mytwo_t *b)
{
	if(a->value < b->value)
		return -1;
	else if(a->value > b->value)
		return 1;
	else
		return 0;
}

static int cmp_descend(
		const mytwo_t *a,
		const mytwo_t *b)
{
	if(a->value < b->value)
		return 1;
	else if(a->value > b->value)
		return -1;
	else
		return 0;
}

static void insert(
		mydata_t *data)
{
	mydata_t *l;
	mydata_find(root, data, &l, NULL, cmp_value);
	mydata_insert(&root, l, data);
}

static int query(
		int value,
		mydata_t **l,
		mydata_t **u)
{
	return mydata_query(root, &value, l, u, cmp_field);
}

static mydata_t *upper(
		mydata_t *data)
{
	mydata_find(root, data, NULL, &data, cmp_value);
	return data;
}


static void example_two()
{
	mytwo_t *ascend = NULL;
	mytwo_t *descend = NULL;
	mytwo_t data[256];
	for(size_t i = 0; i < sizeof(data) / sizeof(*data); i++)
		data[i].value = i % 16;

	for(size_t i = 0; i < 32; i++) {
		mytwo_t *pos;
		myascend_find(ascend, data + i, NULL, &pos, cmp_ascend);
		myascend_insert(&ascend, pos, data + i);

		mydescend_find(descend, data + i, &pos, NULL, cmp_descend);
		mydescend_insert(&descend, pos, data + i);
	}

	printf("ascending:\n");
	for(mytwo_t *cur = myascend_first(ascend); cur; cur = myascend_next(cur))
		printf("IS: %p %d\n", cur, cur->value);
	printf("descending:\n");
	for(mytwo_t *cur = mydescend_first(descend); cur; cur = mydescend_next(cur))
		printf("IS: %p %d\n", cur, cur->value);
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

	mydata_sort(&root, cmp_value);
	printf("SORTED:\n");
	for(cur = mydata_first(root); cur; cur = mydata_next(cur)) 
		printf("%d\n", cur->value);





	example_two();

	return 0;
}

