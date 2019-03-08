#ifndef BINTREE_PREFIX
#define BINTREE_PREFIX bintree
#endif

#ifndef _BINTREE_HDR
#define _BINTREE_HDR
#define BINTREE_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define BINTREE_CONCAT(A, B) A ## _ ## B
#define BINTREE_CONCAT2(A, B) BINTREE_CONCAT(A, B)
#define BINTREE_ID(ID) BINTREE_CONCAT2(BINTREE_PREFIX, ID)
#define BINTREE_NULL ((void*)0)
#define BINTREE_TONODE(X) ((BINTREE_ID(node_t)*)&(X)->BINTREE_FIELD)
#define BINTREE_TOCNODE(X) ((const BINTREE_ID(node_t)*)&(X)->BINTREE_FIELD)
#define BINTREE_L(X) (BINTREE_TONODE(X)->l)
#define BINTREE_R(X) (BINTREE_TONODE(X)->r)
#define BINTREE_P(X) (BINTREE_TONODE(X)->p)
#define BINTREE_B(X) (BINTREE_TONODE(X)->b)
#define BINTREE_SIZE(X) (BINTREE_TONODE(X)->size)
#define BINTREE_CL(X) (BINTREE_TOCNODE(X)->l)
#define BINTREE_CR(X) (BINTREE_TOCNODE(X)->r)
#define BINTREE_CP(X) (BINTREE_TOCNODE(X)->p)
#define BINTREE_CB(X) (BINTREE_TOCNODE(X)->b)
#define BINTREE_CSIZE(X) (BINTREE_TOCNODE(X)->size)
#endif

typedef struct BINTREE_ID(node) BINTREE_ID(node_t);
typedef int (*BINTREE_ID(cmp_t))(const BINTREE_DATA *a, const BINTREE_DATA *b);
typedef int (*BINTREE_ID(qcmp_t))(const BINTREE_DATA *a, const void *b);

struct BINTREE_ID(node) {
#if defined(BINTREE_USE_PARENT)
	BINTREE_DATA *p;
#endif
	BINTREE_DATA *l;
	BINTREE_DATA *r;
#ifdef BINTREE_USE_INDEX
	size_t size;
#endif
#ifdef BINTREE_USE_AVL
	int b;
#endif
};

