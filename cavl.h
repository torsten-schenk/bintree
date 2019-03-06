#pragma once

typedef struct cavl_node cavl_node_t;

#ifdef CAVL_PTRB
#error incomplete implementation
#else
struct cavl_node {
	cavl_node_t *p;
	cavl_node_t *l;
	cavl_node_t *r;
	int b;
};
#endif

#define CAVL_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define CAVL_NULL ((void*)0)

enum {
	CAVL_ROL = 1,
	CAVL_ROR
};

typedef int (*cavl_cmp_t)(const cavl_node_t *a, const cavl_node_t *b);
typedef int (*cavl_qcmp_t)(const cavl_node_t *a, const void *query);
#if 0
typedef int (*cavl_rot_t)(int rot, void *p, void *n, void *c); /* 'rot': one of CAVL_ROL or CAVL_ROR; called after a rotation operation: pre-rotation parent, node and child are handed over as arguments */
typedef int (*cavl_cmpex_t)(const void *a, const void *b, void *user);
typedef int (*cavl_rotex_t)(int rot, void *p, void *n, void *c, void *user); /* 'rot': one of CAVL_ROL or CAVL_ROR; called after a rotation operation: pre-rotation parent, node and child are handed over as arguments */
#endif

#ifdef CAVL_USE_DUMP
static inline void _cavl_rdump(
		const cavl_node_t *n,
		size_t depth,
		void (*print)(const cavl_node_t *node))
{
	if(!n)
		return;
	for(size_t i = 0; i < depth; i++)
		printf("  ");
	if(n->p) {
		if(n->p->l == n)
			printf("L ");
		else
			printf("R ");
	}
	else
		printf("- ");
	printf("b=%d ", n->b);
	print(n);
	printf("\n");
	_cavl_rdump(n->l, depth + 1, print);
	_cavl_rdump(n->r, depth + 1, print);
}

static inline void cavl_dump(
		const cavl_node_t *root,
		void (*print)(const cavl_node_t *node))
{
	_cavl_rdump(root, 0, print);
}
#endif

static inline int cavl_query(
		void *root,
		const void *query, /* guaranteed to be 2nd argument of cmp function */
		void **l,
		void **u,
		cavl_qcmp_t cmp)
{}

static void _cavl_ror(
		cavl_node_t **root,
		cavl_node_t *n)
{
	cavl_node_t *p = n->p;
	cavl_node_t *r = n->r;
	int hl = 0; /* height of left subtree (initially: n->l) */
	int hm = 0; /* height of middle subtree (initially: n->r) */
	int hr = 0; /* height of right subtree (initially: p->r) */

	/* virtual height using balance factors */
	if(n->b > 0)
		hl = n->b;
	else
		hm = -n->b;
	hr = CAVL_MAX(hl, hm) + 1 - p->b;
	p->b = hm - hr;
	n->b = hl - CAVL_MAX(hm, hr) - 1;

	/* rotate */
	if(!p->p)
		*root = n;
	else if(p->p->l == p)
		p->p->l = n;
	else
		p->p->r = n;
	if(r)
		r->p = p;
	n->p = p->p;
	n->r = p;
	p->l = r;
	p->p = n;
}

static void _cavl_rol(
		cavl_node_t **root,
		cavl_node_t *n)
{
	cavl_node_t *p = n->p;
	cavl_node_t *l = n->l;
	int hl = 0; /* height of left subtree (initially: p->l) */
	int hm = 0; /* height of middle subtree (initially: n->l) */
	int hr = 0; /* height of right subtree (initially: n->r) */

	/* virtual height using balance factors */
	if(n->b > 0)
		hm = n->b;
	else
		hr = -n->b;
	hl = CAVL_MAX(hm, hr) + 1 + p->b;
	p->b = hl - hm;
	n->b = CAVL_MAX(hl, hm) + 1 - hr;

	/* rotate */
	if(!p->p)
		*root = n;
	else if(p->p->l == p)
		p->p->l = n;
	else
		p->p->r = n;
	if(l)
		l->p = p;
	n->p = p->p;
	n->l = p;
	p->r = l;
	p->p = n;
}

/* insert 'n' node before 'p' */
static inline void cavl_insert(
		cavl_node_t **root,
		cavl_node_t *p,
		cavl_node_t *n)
{
	cavl_node_t *cur;
#ifdef CAVL_BZERO
	memset(n, 0, sizeof(cavl_node_t));
#endif
	if(!p) { /* append at end */
		if(*root) {
			p = *root;
			for(cur = p; cur->r; cur = cur->r);
			cur->r = n;
			n->p = cur;
		}
		else
			*root = n;
	}
	else if(p->l) { /* insert before p: rightmost position of left subtree of p */
		for(cur = p->l; cur->r; cur = cur->r);
		cur->r = n;
		n->p = cur;
	}
	else { /* insert before p: set left child */
		p->l = n;
		n->p = p;
	}

	/* retracing after insertion */
	for(p = n->p; p; n = p, p = n->p) {
		if(n == p->l) {
			if(!p->b) /* b = 0 */
				p->b++;
			else if(p->b > 0) { /* b = 1 */
				p->b++;
				if(n->b < 0) { /* b = -1 */
					_cavl_rol(root, n->r);
					_cavl_ror(root, n->p);
				}
				else
					_cavl_ror(root, n);
				break;
			}
			else { /* b = -1 */
				p->b++;
				break;
			}
		}
		else {
			if(!p->b) /* b = 0 */
				p->b--;
			else if(p->b < 0) { /* b = -1 */
				p->b--;
				if(n->b > 0) { /* b = 1 */
					_cavl_ror(root, n->l);
					_cavl_rol(root, n->p);
				}
				else
					_cavl_rol(root, n);
				break;
			}
			else { /* b = 1 */
				p->b--;
				break;
			}
		}
	}
}

static inline void cavl_remove(
		cavl_node_t **root,
		cavl_node_t *x)
{
	cavl_node_t *c;
	cavl_node_t *y;
	cavl_node_t *zp;
	int zr = 1; /* used as bool: did we come from right child, i.e. did the right child of zp decrease in height in previous operation? */

	if(x->l && x->r) {
		y = x->l;
		if(y->r) {
			/* descend to rightmost node */
			for(; y->r; y = y->r);
			zp = y->p;

			/* put left child of y to y's position */
			y->p->r = y->l;
			if(y->l)
				y->l->p = y->p;

			/* put y to x's position */
			y->l = x->l;
			y->l->p = y;
		}
		else {
			zp = y;
			zr = 0;
		}
		y->p = x->p;
		y->r = x->r;
		y->b = x->b;
		if(y->r)
			y->r->p = y;
		if(y->p) {
			if(y->p->l == x)
				y->p->l = y;
			else
				y->p->r = y;
		}
		else
			*root = y;
	}
	else {
		zp = x->p;
		if(zp && zp->l == x)
			zr = 0;
		if(!x->p) { /* x was root node, set new root node */
			if(x->l) {
				*root = x->l;
				x->l->p = CAVL_NULL;
			}
			else if(x->r) {
				*root = x->r;
				x->r->p = CAVL_NULL;
			}
			else
				*root = CAVL_NULL;
		}
		else if(x->l) { /* x has only a left subtree and is not root, attach subtree to parent at x's position */
			if(x->p->l == x)
				x->p->l = x->l;
			else
				x->p->r = x->l;
			x->l->p = x->p;
		}
		else if(x->r) { /* x has only a right subtree and is not root, attach subtree to parent at x's position */
			if(x->p->l == x)
				x->p->l = x->r;
			else
				x->p->r = x->r;
			x->r->p = x->p;
		}
		else { /* x has no subtree and is not root, remove x from parent */
			if(x->p->l == x)
				x->p->l = CAVL_NULL;
			else
				x->p->r = CAVL_NULL;
		}
	}

	/* retracing after remove */

	/* if zr: right subtree of zp has been decreased in height
	 * otherwise: left subtree of zp has been decresed in height */
	while(zp) {
		if(zr) {
			if(!zp->b) { /* case 1 */
				zp->b++;
				break;
			}
			else if(zp->b > 0) {
				c = zp->l;
				zp->b++;
				if(!c->b) { /* case 2 */
					_cavl_ror(root, c);
					break;
				}
				else if(c->b < 0) { /* case 3 */
					_cavl_rol(root, c->r);
					_cavl_ror(root, c->p);
				}
				else /* case 4 */
					_cavl_ror(root, c);
			}
			else /* case 5 */
				zp->b++;
		}
		else {
			if(!zp->b) {
				zp->b--;
				break;
			}
			else if(zp->b < 0) {
				c = zp->r;
				zp->b--;
				if(!c->b) {
					_cavl_rol(root, c);
					break;
				}
				else if(c->b > 0) {
					_cavl_ror(root, c->l);
					_cavl_rol(root, c->p);
				}
				else
					_cavl_rol(root, c);
			}
			else
				zp->b--;
		}
		if(zp->p && zp == zp->p->r)
			zr = 1;
		else
			zr = 0;
		zp = zp->p;
	}
}

#if 0
static inline void cavl_sort(
		void **root,
		cavl_cmp_t cmp)
{}

static inline void cavl_insertex(
		void **root,
		void *data,
		cavl_rot_t rot,
		void *user)
{}

static inline void cavl_removeex(
		void **root,
		void *n,
		cavl_rotex_t rot,
		void *user)
{}

static inline void cavl_sortex(
		void **root,
		cavl_cmp_t cmp,
		cavl_rot_t rot,
		void *user)
{}
#endif

