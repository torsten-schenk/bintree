#pragma once

typedef struct bintree_avlnode bintree_avlnode_t;

#define BINTREE_AVLNODE_HEADER \
	BINTREE_NODE_HEADER \
	int b;

struct bintree_avlnode {
	BINTREE_NODE_AVLHEADER
};


