#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CAVL_USE_DUMP
#include "cavl.h"

typedef struct {
	cavl_node_t cavl;
	int id;
} node_t;

static void print(
		const cavl_node_t *n_)
{
	const node_t *n = (node_t*)n_;
	printf("%d", n->id);
}

static int qcmp(
		const cavl_node_t *a_,
		const void *q_)
{
	const node_t *a = (node_t*)a_;
	const int *q = q_;

	if(a->id < *q)
		return -1;
	else if(a->id > *q)
		return 1;
	else
		return 0;
}

int main()
{
	cavl_node_t *root = NULL;
	node_t *cur;
	node_t a[16];
	cavl_node_t *l;
	cavl_node_t *u;
	int id;

	memset(a, 0, sizeof(a));
	for(size_t i = 0; i < sizeof(a) / sizeof(*a); i++) {
		a[i].id = i;
		cavl_insert(&root, NULL, (cavl_node_t*)(a + i));
	}
	cavl_dump(root, print);

	for(cur = (node_t*)cavl_first(root); cur; cur = (node_t*)cavl_next((cavl_node_t*)cur))
		printf("%d\n", cur->id);

	id = 8;
	cavl_query(root, &id, &l, &u, qcmp);
	printf("GOT: %d %d\n", ((node_t*)l)->id, ((node_t*)u)->id);

/*	for(size_t k = 0; k < sizeof(a) / sizeof(*a); k++) {
		printf("REMOVE: %zu\n", k);
		root = NULL;
		memset(a, 0, sizeof(a));
		for(size_t i = 0; i < sizeof(a) / sizeof(*a); i++) {
			a[i].id = i;
			cavl_insert(&root, root, (cavl_node_t*)(a + i));
		}
		//cavl_dump(root, print);
		cavl_remove(&root, (cavl_node_t*)(a + k));
	}*/

	return 0;
}

