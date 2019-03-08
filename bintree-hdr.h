#ifndef BINTREE_PREFIX
#define BINTREE_PREFIX bintree
#endif

#ifndef _BINTREE_HDR
#define _BINTREE_HDR
#define BINTREE_CONCAT(A, B) A ## _ ## B
#define BINTREE_CONCAT2(A, B) BINTREE_CONCAT(A, B)
#define BINTREE_ID(ID) BINTREE_CONCAT2(BINTREE_PREFIX, ID)
#define BINTREE_NULL ((void*)0)
#define BINTREE_TONODE(X) ((BINTREE_ID(node_t)*)&(X)->BINTREE_FIELD)
#define BINTREE_TOCNODE(X) ((const BINTREE_ID(node_t)*)&(X)->BINTREE_FIELD)
#endif

typedef struct BINTREE_ID(node) BINTREE_ID(node_t);
typedef int (*BINTREE_ID(cmp_t))(const BINTREE_DATA *a, const BINTREE_DATA *b);

struct BINTREE_ID(node) {
#if defined(BINTREE_USE_PARENT)
	BINTREE_DATA *p;
#endif
	BINTREE_DATA *l;
	BINTREE_DATA *r;
#ifdef BINTREE_USE_AVL
	int b;
#endif
#ifdef BINTREE_USE_INDEX
	size_t size;
#endif
};

