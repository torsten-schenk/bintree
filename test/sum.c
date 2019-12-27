#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct mydata mydata_t;

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

#define BINTREE_CONFIG MYDATA
#include <bintree-impl.h>

#define N 1000000
#define NREMOVE 900000

static mydata_t data[N];
static size_t rnd[N];

static int cmp_data(
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

static mydata_t *getrnd(
		size_t n)
{
	size_t pos = rand() % n;
	size_t v = rnd[pos];
	rnd[pos] = rnd[n - 1];
	rnd[n - 1] = v;
	return data + v;
}

static void tgf_node(
		FILE *out,
		const mydata_t *n,
		size_t *id)
{
	if(n) {
		fprintf(out, "%zu %zu/%zu\n", ++*id, n->value, n->node.cumul);
		tgf_node(out, n->node.l, id);
		tgf_node(out, n->node.r, id);
	}
}

static void tgf_link(
		FILE *out,
		const mydata_t *n,
		size_t *id)
{
	if(n) {
		size_t src = ++*id;
		if(n->node.l)
			fprintf(out, "%zu %zu\n", src, *id + 1);
		tgf_link(out, n->node.l, id);
		if(n->node.r)
			fprintf(out, "%zu %zu\n", src, *id + 1);
		tgf_link(out, n->node.r, id);
	}
}

static void totgf(
		FILE *out,
		mydata_t *root)
{
	size_t id;
	id = 0;
	tgf_node(out, root, &id);
	fprintf(out, "#\n");
	id = 0;
	tgf_link(out, root, &id);
}

int main()
{
	size_t total = 0;
	size_t pre = 0;
	size_t cpre;
	size_t cpost;
	mydata_t *root = NULL;
	for(size_t i = 0; i < N; i++) {
		data[i].value = i;
		rnd[i] = i;
	}

	/* insert all elements in a random order */
	for(size_t i = 0; i < N; i++) {
		mydata_t *piv;
		mydata_t *cur = getrnd(N - i);
		assert(!cur->inserted);
		cur->inserted = 1;
		total += data[i].value;
		mydata_find(root, cur, NULL, &piv, cmp_data);
		mydata_insert(&root, piv, cur);
	}

//	totgf(stdout, root);
//	return 0;

	/* check, if sums are ok after insertion */
	for(size_t i = 0; i < N; i++) {
		mydata_sum(data + i, &cpre, &cpost);
//		printf("SUM: %d %zu==%zu %zu==%zu\n", i, cpre, i * (i - 1) / 2, cpost, (i + N) * (N - i - 1) / 2);
		assert(cpre == i * (i - 1) / 2);
//		printf("CHECK: %zu\n", (i + N) * (N - i - 1) / 2);
		assert(cpost == (i + N - 1) * (N - i) / 2);
	}

	/* randomly remove elements */
	for(size_t i = 0; i < NREMOVE; i++) {
		mydata_t *cur = getrnd(N - i);
		cur->inserted = 0;
		total -= cur->value;
		mydata_remove(&root, cur);
	}

	/* check, if sums still are ok */
	for(size_t i = 0; i < N; i++) {
		mydata_t *cur = data + i;
		if(cur->inserted) {
			mydata_sum(cur, &cpre, &cpost);
			assert(cpre == pre);
			assert(cpost == total - pre);
//			printf("SUM: %d %zu==%zu %zu==%zu\n", i, mydata_presum(data + i), pre, mydata_postsum(data + i), total - pre - cur->value);
			pre += cur->value;
		}
	}

	return 0;
}

