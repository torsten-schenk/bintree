#pragma once

typedef struct bintree_node bintree_node_t;
typedef int (*bintree_cmp_t)(const bintree_node_t *a, const bintree_node_t *b);
typedef int (*bintree_qcmp_t)(const bintree_node_t *a, const void *b);

#define BINTREE_NODE_HEADER \
	void *p; \
	void *l; \
	void *r;

struct bintree_node {
	BINTREE_NODE_HEADER
};

#define BINTREE_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define BINTREE_NULL ((void*)0)

static inline int bintree_query(
		bintree_node_t *root,
		const void *query,
		bintree_node_t **lret,
		bintree_node_t **uret,
		bintree_qcmp_t cmpfn)
{
	int cmp;
	int ret = 0;
	bintree_node_t *n;
	bintree_node_t *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, query);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = n->l;
			}
			else
				n = n->r;
		}
		if(lret)
			*lret = c;
	}
	if(uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, query);
			if(cmp > 0) {
				c = n;
				n = n->l;
			}
			else {
				if(!cmp)
					ret = 1;
				n = n->r;
			}
		}
		*uret = c;
	}
	return ret;
}

static inline bintree_node_t *bintree_first(
		bintree_node_t *n)
{
	if(n)
		for(; n->l; n = n->l);
	return n;
}

static inline bintree_node_t *bintree_last(
		bintree_node_t *n)
{
	if(n)
		for(; n->r; n = n->r);
	return n;
}

static inline bintree_node_t *bintree_next(
		bintree_node_t *n)
{
	if(n->r)
		for(n = n->r; n->l; n = n->l);
	else {
		for(; n->p && n == n->p->r; n = n->p);
		n = n->p;
	}
	return n;
}

static inline bintree_node_t *bintree_prev(
		bintree_node_t *n)
{
	if(n->l)
		for(n = n->l; n->r; n = n->r);
	else {
		for(; n->p && n == n->p->l; n = n->p);
		n = n->p;
	}
	return n;
}

