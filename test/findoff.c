#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct mydata mydata_t;
typedef struct test test_t;

#define MYDATA_PREFIX mydata
#define MYDATA_FN static inline
#define MYDATA_DATA mydata_t
#define MYDATA_FIELD node
#define MYDATA_USE_PARENT 1
#define MYDATA_USE_SUM 1
#define MYDATA_USE_AVL 1
#define MYDATA_USE_BZERO 1
#define MYDATA_SUM size_t
#define MYDATA_VALUE value

struct mydata {
#define BINTREE_CONFIG MYDATA
#include <bintree-hdr.h>
	size_t value;
	int inserted;
};

struct test {
	size_t off;
	bool ret;
	const mydata_t *l;
	size_t loff;
	const mydata_t *u;
	size_t uoff;
};

#define BINTREE_CONFIG MYDATA
#include <bintree-impl.h>

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(*(X)))
static mydata_t data_a[] = {
	{ .value = 0 },
	{ .value = 1 },
	{ .value = 0 },
	{ .value = 2 },
	{ .value = 0 }
};

static const test_t tests_a[] = {
 { 0, true, data_a + 0, 0, data_a + 1, 0 },
 { 1, true, data_a + 1, 1, data_a + 3, 0 },
 { 2, true, data_a + 3, 1, data_a + 3, 1 },
 { 3, false, data_a + 3, 2, NULL, 0 },
 { 4, false, NULL, 0, NULL, 0 },
};

static mydata_t data_b[] = {
	{ .value = 1 },
	{ .value = 2 },
	{ .value = 3 }
};

static const test_t tests_b[] = {
 { 0, true, data_b + 0, 0, data_b + 0, 0 },
 { 1, true, data_b + 0, 1, data_b + 1, 0 },
};

static void run(
		mydata_t *data,
		size_t ndata,
		const test_t *tests,
		size_t ntest)
{
	mydata_t *root = NULL;
	for(size_t i = 0; i < ndata; i++)
		mydata_insert(&root, NULL, data + i);

	for(size_t i = 0; i < ntest; i++) {
		mydata_t *l, *u;
		size_t loff, uoff;
		int ret = mydata_findoff(root, tests[i].off, &l, &loff, &u, &uoff);
		assert(!ret == !tests[i].ret);
		assert(l == tests[i].l);
		if(tests[i].l) {
			assert(loff == tests[i].loff);
		}
		assert(u == tests[i].u);
		if(tests[i].u) {
			assert(uoff == tests[i].uoff);
		}
	}
}

int main()
{
	run(data_a, ARRAY_SIZE(data_a), tests_a, ARRAY_SIZE(tests_a));
	run(data_b, ARRAY_SIZE(data_b), tests_b, ARRAY_SIZE(tests_b));
}

