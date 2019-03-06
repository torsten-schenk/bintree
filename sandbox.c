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

int main()
{
	cavl_node_t *root = NULL;
	node_t a[1000];

/*	memset(a, 0, sizeof(a));
	for(size_t i = 0; i < sizeof(a) / sizeof(*a); i++) {
		a[i].id = i;
		cavl_insert(&root, root, (cavl_node_t*)(a + i));
	}
	cavl_dump(root, print);
	cavl_remove(&root, (cavl_node_t*)(a + 19));
	cavl_dump(root, print);*/

	for(size_t k = 0; k < sizeof(a) / sizeof(*a); k++) {
		printf("REMOVE: %zu\n", k);
		root = NULL;
		memset(a, 0, sizeof(a));
		for(size_t i = 0; i < sizeof(a) / sizeof(*a); i++) {
			a[i].id = i;
			cavl_insert(&root, root, (cavl_node_t*)(a + i));
		}
		//cavl_dump(root, print);
		cavl_remove(&root, (cavl_node_t*)(a + k));
	}

	return 0;
}

