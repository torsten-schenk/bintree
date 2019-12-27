#ifndef BINTREE_CONFIG
#error missing BINTREE_CONFIG before include statement
#else

#define BINTREE_PREFIX BINTREE_SCONCAT2(BINTREE_CONFIG, _PREFIX)
#define BINTREE_FIELD BINTREE_SCONCAT2(BINTREE_CONFIG, _FIELD)
#define BINTREE_DATA BINTREE_SCONCAT2(BINTREE_CONFIG, _DATA)
#define BINTREE_FN BINTREE_SCONCAT2(BINTREE_CONFIG, _FN)

#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_MULTI) != 0
#define BINTREE_USE_MULTI
#define BINTREE_MULTI BINTREE_SCONCAT2(BINTREE_CONFIG, _MULTI)
#define BINTREE_TONODE(X) ((X)->BINTREE_FIELD + multi)
#define BINTREE_L(X) (X->BINTREE_FIELD[multi].l)
#define BINTREE_R(X) (X->BINTREE_FIELD[multi].r)
#define BINTREE_P(X) (X->BINTREE_FIELD[multi].p)
#define BINTREE_B(X) (X->BINTREE_FIELD[multi].b)
#define BINTREE_SIZE(X) (X->BINTREE_FIELD[multi].size)
#define BINTREE_CUMUL(X) (X->BINTREE_FIELD[multi].cumul)
#define BINTREE_CALL(X, ...) BINTREE_ID(X)(multi, __VA_ARGS__)
#else
#define BINTREE_TONODE(X) (&(X)->BINTREE_FIELD)
#define BINTREE_L(X) (X->BINTREE_FIELD.l)
#define BINTREE_R(X) (X->BINTREE_FIELD.r)
#define BINTREE_P(X) (X->BINTREE_FIELD.p)
#define BINTREE_B(X) (X->BINTREE_FIELD.b)
#define BINTREE_SIZE(X) (X->BINTREE_FIELD.size)
#define BINTREE_CUMUL(X) (X->BINTREE_FIELD.cumul)
#define BINTREE_CALL(X, ...) BINTREE_ID(X)(__VA_ARGS__)
#endif

#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_QCMPARG) != 0
#define BINTREE_USE_QCMPARG
#define BINTREE_QCMPARG BINTREE_SCONCAT2(BINTREE_CONFIG, _QCMPARG)
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_FCMPARG) != 0
#define BINTREE_USE_FCMPARG
#define BINTREE_FCMPARG BINTREE_SCONCAT2(BINTREE_CONFIG, _FCMPARG)
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_CMPARG) != 0
#define BINTREE_USE_QCMPARG
#define BINTREE_USE_FCMPARG
#define BINTREE_QCMPARG BINTREE_SCONCAT2(BINTREE_CONFIG, _CMPARG)
#define BINTREE_FCMPARG BINTREE_SCONCAT2(BINTREE_CONFIG, _CMPARG)
#endif

#ifdef BINTREE_USE_QCMPARG
typedef int (*BINTREE_ID(qcmp_t))(const BINTREE_DATA *a, const void *b, BINTREE_QCMPARG cmparg);
#define BINTREE_QCMP(CMP, A, B) (CMP(A, B, cmparg))
#else
typedef int (*BINTREE_ID(qcmp_t))(const BINTREE_DATA *a, const void *b);
#define BINTREE_QCMP(CMP, A, B) (CMP(A, B))
#endif
#ifdef BINTREE_USE_FCMPARG
typedef int (*BINTREE_ID(cmp_t))(const BINTREE_DATA *a, const BINTREE_DATA *b, BINTREE_FCMPARG cmparg);
#define BINTREE_FCMP(CMP, A, B) (CMP(A, B, cmparg))
#else
typedef int (*BINTREE_ID(cmp_t))(const BINTREE_DATA *a, const BINTREE_DATA *b);
#define BINTREE_FCMP(CMP, A, B) (CMP(A, B))
#endif

#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_PARENT) != 0
#define BINTREE_USE_PARENT
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_INDEX) != 0
#define BINTREE_USE_INDEX
#define BINTREE_INDEX BINTREE_SCONCAT2(BINTREE_CONFIG, _INDEX)
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_SUM) != 0
#define BINTREE_USE_SUM
#define BINTREE_SUM BINTREE_SCONCAT2(BINTREE_CONFIG, _SUM)
#define BINTREE_VALUE(X) (X->BINTREE_SCONCAT2(BINTREE_CONFIG, _VALUE))
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_AVL) != 0
#define BINTREE_USE_AVL
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_BZERO) != 0
#define BINTREE_USE_BZERO
#endif


#ifdef BINTREE_DEBUG
BINTREE_FN void BINTREE_ID(dump) (
		const BINTREE_DATA *n,
		int indent)
{
	if(!n)
		return;
	for(int i = 0; i < indent; i++)
		printf("  ");
	printf("node=%p b=%d p=%p l=%p r=%p\n", n, BINTREE_B(n), BINTREE_P(n), BINTREE_L(n), BINTREE_R(n));
	BINTREE_ID(dump)(BINTREE_L(n), indent + 1);
	BINTREE_ID(dump)(BINTREE_R(n), indent + 1);
}

BINTREE_FN void BINTREE_ID(validate) (
		const BINTREE_DATA *n)
{
	const BINTREE_DATA *c;
	if(!n)
		return;
	c = BINTREE_L(n);
	if(c && BINTREE_P(c) != n)
		printf("left child of %p has invalid parent\n", n);
	BINTREE_ID(validate)(c);
	c = BINTREE_R(n);
	if(c && BINTREE_P(c) != n)
		printf("right child of %p has invalid parent\n", n);
	BINTREE_ID(validate)(c);
}
#endif

BINTREE_FN void BINTREE_ID(bzero) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n
)
{
	bzero(BINTREE_TONODE(n), sizeof(*BINTREE_TONODE(n)));
}

BINTREE_FN int BINTREE_ID(memberof) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *root,
		const BINTREE_DATA *n
)
{
	for(; n; n = BINTREE_P(n))
		if(n == root)
			return 1;
	return 0;
}

/* NOTE: query() and find() are very similar, the only difference is the cmp function for convenience and type safety */
BINTREE_FN int BINTREE_ID(query) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		const void *query,
		BINTREE_DATA **lret,
		BINTREE_DATA **uret,
		BINTREE_ID(qcmp_t) cmpfn
#ifdef BINTREE_USE_QCMPARG
		, BINTREE_QCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp > 0) {
				c = n;
				n = BINTREE_L(n);
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
			}
		}
		*uret = c;
	}
	return ret;
}

BINTREE_FN int BINTREE_ID(find) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		const BINTREE_DATA *data,
		BINTREE_DATA **lret,
		BINTREE_DATA **uret,
		BINTREE_ID(cmp_t) cmpfn
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp > 0) {
				c = n;
				n = BINTREE_L(n);
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
			}
		}
		*uret = c;
	}
	return ret;
}

BINTREE_FN BINTREE_DATA *BINTREE_ID(first)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_L(n); n = BINTREE_L(n));
	return n;
}

BINTREE_FN BINTREE_DATA *BINTREE_ID(last)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_R(n); n = BINTREE_R(n));
	return n;
}

#ifdef BINTREE_USE_PARENT
BINTREE_FN BINTREE_DATA *BINTREE_ID(root)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	for(; BINTREE_P(n); n = BINTREE_P(n));
	return n;
}

BINTREE_FN BINTREE_DATA *BINTREE_ID(next)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	if(BINTREE_R(n))
		for(n = BINTREE_R(n); BINTREE_L(n); n = BINTREE_L(n));
	else {
		for(; BINTREE_P(n) && n == BINTREE_R(BINTREE_P(n)); n = BINTREE_P(n));
		n = BINTREE_P(n);
	}
	return n;
}

BINTREE_FN BINTREE_DATA *BINTREE_ID(prev)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	if(BINTREE_L(n))
		for(n = BINTREE_L(n); BINTREE_R(n); n = BINTREE_R(n));
	else {
		for(; BINTREE_P(n) && n == BINTREE_L(BINTREE_P(n)); n = BINTREE_P(n));
		n = BINTREE_TONODE(n)->p;
	}
	return n;
}

BINTREE_FN void BINTREE_ID(ror)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *n)
{
	BINTREE_DATA *p = BINTREE_P(n);
	BINTREE_DATA *r = BINTREE_R(n);

#ifdef BINTREE_USE_AVL
	int hl = 0; /* height of left subtree (initially: n->l) */
	int hm = 0; /* height of middle subtree (initially: n->r) */
	int hr = 0; /* height of right subtree (initially: p->r) */
#endif

#ifdef BINTREE_USE_INDEX
	BINTREE_INDEX sl; /* size of subtree rooted in n->l */
	BINTREE_INDEX sm; /* size of subtree rooted in n->r */
	BINTREE_INDEX sr; /* size of subtree rooted in p->r */
#endif

#ifdef BINTREE_USE_SUM
	BINTREE_SUM cl; /* cumulative sum of subtree rooted in n->l */
	BINTREE_SUM cm; /* cumulative sum of subtree rooted in n->r */
	BINTREE_SUM cr; /* cumulative sum of subtree rooted in p->r */
#endif

#ifdef BINTREE_USE_INDEX
	/* calculate sizes of the three subtrees and set the new sizue for p and n */
	if(BINTREE_L(n))
		sl = BINTREE_SIZE(BINTREE_L(n));
	else
		sl = 0;
	sm = BINTREE_SIZE(n) - sl - 1;
	sr = BINTREE_SIZE(p) - sl - sm - 2;
	BINTREE_SIZE(n) = sl + sm + sr + 2;
	BINTREE_SIZE(p) = sm + sr + 1; 
#endif

#ifdef BINTREE_USE_SUM
	/* calculate sum of the three subtrees and set the new sizue for p and n */
	if(BINTREE_L(n))
		cl = BINTREE_CUMUL(BINTREE_L(n));
	else
		cl = 0;
	cm = BINTREE_CUMUL(n) - cl - BINTREE_VALUE(n);
	cr = BINTREE_CUMUL(p) - cl - cm - BINTREE_VALUE(n) - BINTREE_VALUE(p);
	BINTREE_CUMUL(n) = cl + cm + cr + BINTREE_VALUE(n) + BINTREE_VALUE(p);
	BINTREE_CUMUL(p) = cm + cr + BINTREE_VALUE(p); 
#endif

#ifdef BINTREE_USE_AVL
	/* virtual height using balance factors */
	if(BINTREE_B(n) > 0)
		hl = BINTREE_B(n);
	else
		hm = -BINTREE_B(n);
	hr = BINTREE_MAX(hl, hm) + 1 - BINTREE_B(p);
	BINTREE_B(p) = hm - hr;
	BINTREE_B(n) = hl - BINTREE_MAX(hm, hr) - 1;
#endif

	/* rotate */
	if(!BINTREE_P(p))
		*root = n;
	else if(BINTREE_L(BINTREE_P(p)) == p)
		BINTREE_L(BINTREE_P(p)) = n;
	else
		BINTREE_R(BINTREE_P(p)) = n;
	if(r)
		BINTREE_P(r) = p;
	BINTREE_P(n) = BINTREE_P(p);
	BINTREE_R(n) = p;
	BINTREE_L(p) = r;
	BINTREE_P(p) = n;
}

BINTREE_FN void BINTREE_ID(rol)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *n)
{
	BINTREE_DATA *p = BINTREE_P(n);
	BINTREE_DATA *l = BINTREE_L(n);

#ifdef BINTREE_USE_AVL
	int hl = 0; /* height of left subtree (initially: p->l) */
	int hm = 0; /* height of middle subtree (initially: n->l) */
	int hr = 0; /* height of right subtree (initially: n->r) */
#endif

#ifdef BINTREE_USE_INDEX
	BINTREE_INDEX sl; /* size of subtree rooted in p->l */
	BINTREE_INDEX sm; /* size of subtree rooted in n->l */
	BINTREE_INDEX sr; /* size of subtree rooted in n->r */
#endif

#ifdef BINTREE_USE_SUM
	BINTREE_SUM cl;
	BINTREE_SUM cm;
	BINTREE_SUM cr;
#endif

#ifdef BINTREE_USE_INDEX
	/* calculate sizes of the three subtrees and set the new sizue for p and n */
	if(BINTREE_R(n))
		sr = BINTREE_SIZE(BINTREE_R(n));
	else
		sr = 0;
	sm = BINTREE_SIZE(n) - sr - 1; /* TODO is this equal to BINTREE_SIZE(BINTREE_L(n))? (see also comment and other rotation) */
	sl = BINTREE_SIZE(p) - sm - sr - 2;
	BINTREE_SIZE(n) = sl + sm + sr + 2;
	BINTREE_SIZE(p) = sl + sm + 1; 
#endif

#ifdef BINTREE_USE_SUM
	if(BINTREE_R(n))
		cr = BINTREE_CUMUL(BINTREE_R(n));
	else
		cr = 0;
	cm = BINTREE_CUMUL(n) - cr - BINTREE_VALUE(n);
	cl = BINTREE_CUMUL(p) - cm - cr - BINTREE_VALUE(n) - BINTREE_VALUE(p);
	BINTREE_CUMUL(n) = cl + cm + cr + BINTREE_VALUE(n) + BINTREE_VALUE(p);
	BINTREE_CUMUL(p) = cl + cm + BINTREE_VALUE(p); 
#endif

#ifdef BINTREE_USE_AVL
	/* virtual height using balance factors */
	if(BINTREE_B(n) > 0)
		hm = BINTREE_B(n);
	else
		hr = -BINTREE_B(n);
	hl = BINTREE_MAX(hm, hr) + 1 + BINTREE_B(p);
	BINTREE_B(p) = hl - hm;
	BINTREE_B(n) = BINTREE_MAX(hl, hm) + 1 - hr;
#endif

	/* rotate */
	if(!BINTREE_P(p))
		*root = n;
	else if(BINTREE_L(BINTREE_P(p)) == p)
		BINTREE_L(BINTREE_P(p)) = n;
	else
		BINTREE_R(BINTREE_P(p)) = n;
	if(l)
		BINTREE_P(l) = p;
	BINTREE_P(n) = BINTREE_P(p);
	BINTREE_L(n) = p;
	BINTREE_R(p) = l;
	BINTREE_P(p) = n;
}

/* insert 'n' node before 'p' */
BINTREE_FN void BINTREE_ID(insert)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *p,
		BINTREE_DATA *n)
{
	BINTREE_DATA *cur;
#ifdef BINTREE_USE_BZERO
	bzero(BINTREE_TONODE(n), sizeof(*BINTREE_TONODE(n)));
#endif
	if(!p) { /* append at end */
		if(*root) {
			p = *root;
			for(cur = p; BINTREE_R(cur); cur = BINTREE_R(cur));
			BINTREE_R(cur) = n;
			BINTREE_P(n) = cur;
		}
		else
			*root = n;
	}
	else if(BINTREE_L(p)) { /* insert before p: rightmost position of left subtree of p */
		for(cur = BINTREE_L(p); BINTREE_R(cur); cur = BINTREE_R(cur));
		BINTREE_R(cur) = n;
		BINTREE_P(n) = cur;
	}
	else { /* insert before p: set left child */
		BINTREE_L(p) = n;
		BINTREE_P(n) = p;
	}

#ifdef BINTREE_USE_INDEX
	for(p = n; p; p = BINTREE_P(p))
		BINTREE_SIZE(p)++;
#endif

#ifdef BINTREE_USE_SUM
	for(p = n; p; p = BINTREE_P(p))
		BINTREE_CUMUL(p) += BINTREE_VALUE(n);
#endif

#ifdef BINTREE_USE_AVL
	/* retracing after insertion */
	for(p = BINTREE_P(n); p; n = p, p = BINTREE_P(n)) {
		if(n == BINTREE_L(p)) {
			if(!BINTREE_B(p)) /* b = 0 */
				BINTREE_B(p)++;
			else if(BINTREE_B(p) > 0) { /* b = 1 */
				BINTREE_B(p)++;
				if(BINTREE_B(n) < 0) { /* b = -1 */
					BINTREE_CALL(rol, root, BINTREE_R(n));
					BINTREE_CALL(ror, root, BINTREE_P(n));
				}
				else
					BINTREE_CALL(ror, root, n);
				break;
			}
			else { /* b = -1 */
				BINTREE_B(p)++;
				break;
			}
		}
		else {
			if(!BINTREE_B(p)) /* b = 0 */
				BINTREE_B(p)--;
			else if(BINTREE_B(p) < 0) { /* b = -1 */
				BINTREE_B(p)--;
				if(BINTREE_B(n) > 0) { /* b = 1 */
					BINTREE_CALL(ror, root, BINTREE_L(n));
					BINTREE_CALL(rol, root, BINTREE_P(n));
				}
				else
					BINTREE_CALL(rol, root, n);
				break;
			}
			else { /* b = 1 */
				BINTREE_B(p)--;
				break;
			}
		}
	}
#endif
}

BINTREE_FN void BINTREE_ID(remove)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *x)
{
	BINTREE_DATA *c;
	BINTREE_DATA *y;
	BINTREE_DATA *zp;
#ifdef BINTREE_USE_AVL
	int zr = 1; /* used as bool: did we come from right child, i.e. did the right child of zp decrease in height in previous operation? */
#endif

	if(BINTREE_L(x) && BINTREE_R(x)) {
		/* find predecessor of x */
		y = BINTREE_L(x);
		if(BINTREE_R(y)) { /* case 1 */
			/* descend to rightmost node */
			for(; BINTREE_R(y); y = BINTREE_R(y));
			zp = BINTREE_P(y);

			/* put left child of y to y's position */
			BINTREE_R(BINTREE_P(y)) = BINTREE_L(y);
			if(BINTREE_L(y))
				BINTREE_P(BINTREE_L(y)) = BINTREE_P(y);

			/* put y to x's position */
			BINTREE_L(y) = BINTREE_L(x);
			BINTREE_P(BINTREE_L(y)) = y;
		}
		else { /* case 2 */
			zp = y;
#ifdef BINTREE_USE_AVL
			zr = 0;
#endif
		}
		/* update auxiliary data of y */
#ifdef BINTREE_USE_AVL
		BINTREE_B(y) = BINTREE_B(x);
#endif
#ifdef BINTREE_USE_INDEX
		BINTREE_SIZE(y) = BINTREE_SIZE(x);
#endif
#ifdef BINTREE_USE_SUM
		BINTREE_CUMUL(y) = BINTREE_CUMUL(x);
		for(c = zp; c != y; c = BINTREE_P(c))
			BINTREE_CUMUL(c) -= BINTREE_VALUE(y);
#endif
		/* update tree pointers for y movement */
		BINTREE_P(y) = BINTREE_P(x);
		BINTREE_R(y) = BINTREE_R(x);
		if(BINTREE_R(y))
			BINTREE_P(BINTREE_R(y)) = y;
		if(BINTREE_P(y)) {
			if(BINTREE_L(BINTREE_P(y)) == x)
				BINTREE_L(BINTREE_P(y)) = y;
			else
				BINTREE_R(BINTREE_P(y)) = y;
		}
		else
			*root = y;
	}
	else {
		zp = BINTREE_P(x);
#ifdef BINTREE_USE_SUM
		y = zp;
#endif
#ifdef BINTREE_USE_AVL
		if(zp && BINTREE_L(zp) == x)
			zr = 0;
#endif
		if(!BINTREE_P(x)) { /* x was root node, set new root node */
			if(BINTREE_L(x)) { /* remove case 3, ZP is root */
				*root = BINTREE_L(x);
				BINTREE_P(BINTREE_L(x)) = BINTREE_NULL;
			}
			else if(BINTREE_R(x)) { /* remove case 4, ZP is root */
				*root = BINTREE_R(x);
				BINTREE_P(BINTREE_R(x)) = BINTREE_NULL;
			}
			else
				*root = BINTREE_NULL;
		}
		else if(BINTREE_L(x)) { /* remove case 3: x has only a left subtree and is not root, attach subtree to parent at x's position */
			if(BINTREE_L(BINTREE_P(x)) == x)
				BINTREE_L(BINTREE_P(x)) = BINTREE_L(x);
			else
				BINTREE_R(BINTREE_P(x)) = BINTREE_L(x);
			BINTREE_P(BINTREE_L(x)) = BINTREE_P(x);
		}
		else if(BINTREE_R(x)) { /* x remove case 4: has only a right subtree and is not root, attach subtree to parent at x's position */
			if(BINTREE_L(BINTREE_P(x)) == x)
				BINTREE_L(BINTREE_P(x)) = BINTREE_R(x);
			else
				BINTREE_R(BINTREE_P(x)) = BINTREE_R(x);
			BINTREE_P(BINTREE_R(x)) = BINTREE_P(x);
		}
		else { /* x has no subtree and is not root, remove x from parent */
			if(BINTREE_L(BINTREE_P(x)) == x)
				BINTREE_L(BINTREE_P(x)) = BINTREE_NULL;
			else
				BINTREE_R(BINTREE_P(x)) = BINTREE_NULL;
		}
	}

#ifdef BINTREE_USE_INDEX
	for(c = zp; c; c = BINTREE_P(c))
		BINTREE_SIZE(c)--;
#endif
#ifdef BINTREE_USE_SUM
	for(c = y; c; c = BINTREE_P(c))
		BINTREE_CUMUL(c) -= BINTREE_VALUE(x);
#endif
#ifdef BINTREE_USE_BZERO
	bzero(BINTREE_TONODE(x), sizeof(*BINTREE_TONODE(x)));
#endif

#ifdef BINTREE_USE_AVL
	/* retracing after remove */

	/* if zr: right subtree of zp has been decreased in height
	 * otherwise: left subtree of zp has been decresed in height */
	while(zp) {
		/* zp is the root of the subtree to be updated. after update, this must still hold. */
		if(zr) {
			if(!BINTREE_B(zp)) { /* avl case 1 */
				BINTREE_B(zp)++;
				break; /* no further updates required */
			}
			else if(BINTREE_B(zp) > 0) {
				c = BINTREE_L(zp);
				BINTREE_B(zp)++;
				if(!BINTREE_B(c)) { /* avl case 2 */
					BINTREE_CALL(ror, root, c);
					break; /* no further updates required */
				}
				else if(BINTREE_B(c) < 0) { /* avl case 3 */
					zp = BINTREE_R(c);
					BINTREE_CALL(rol, root, zp);
					BINTREE_CALL(ror, root, zp);
				}
				else { /* avl case 4 */
					zp = c;
					BINTREE_CALL(ror, root, zp);
				}
			}
			else /* avl case 5 */
				BINTREE_B(zp)++;
		}
		else {
			if(!BINTREE_B(zp)) { /* avl case 1 */
				BINTREE_B(zp)--;
				break; /* no further updates required */
			}
			else if(BINTREE_B(zp) < 0) {
				c = BINTREE_R(zp);
				BINTREE_B(zp)--;
				if(!BINTREE_B(c)) { /* avl case 2 */
					BINTREE_CALL(rol, root, c);
					break; /* no further updates required */
				}
				else if(BINTREE_B(c) > 0) { /* avl case 3 */
					zp = BINTREE_L(c);
					BINTREE_CALL(ror, root, zp);
					BINTREE_CALL(rol, root, zp);
				}
				else { /* avl case 4 */
					zp = c;
					BINTREE_CALL(rol, root, zp);
				}
			}
			else /* avl case 5 */
				BINTREE_B(zp)--;
		}
		if(BINTREE_P(zp) && zp == BINTREE_R(BINTREE_P(zp)))
			zr = 1;
		else
			zr = 0;
		zp = BINTREE_P(zp);
	}
#endif
}

/* take one node from bintree. use case: destroy whole tree using a loop. note that no rebalancing/metadata updates will take place, since the purpose is to destroy the tree structure.
 * decon() retrieves the elements from first to last in sequential order:
 * 1. we descend to first element in remaining tree
 * 2. this is the node n we return
 * 3. a) n has right subtree r:
 *       leftmost element in r is next element which must be returned
 *       --> put r into n's position; parent of n is ordered after whole subtree r, so we ascend to n's parent after r is deconstructed
 *       --> remember r, since in next iteration, we perform 1.
 *    b) n has no right subtree (also no left subtree; therefore n is a leaf node):
 *       parent p is next in-order element
 *       --> remember p
 *       --> in next iteration there will be no left child anymore */
BINTREE_FN BINTREE_DATA *BINTREE_ID(decon)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **it)
{
	BINTREE_DATA *n = *it;
	BINTREE_DATA *r;
	BINTREE_DATA *p;

	if(!n)
		return BINTREE_NULL;
	for(; BINTREE_L(n); n = BINTREE_L(n));
	p = BINTREE_P(n);
	r = BINTREE_R(n);

	/* purpose here is to put r into n's position (as left child of p) and continue with p. */
	if(r) {
		BINTREE_P(r) = p;
		*it = r; /* we know that we will descend into r again in next loop iteration, so continue with r. it is also possible that there is no parent, so it is mandatory to store r here. */
	}
	else
		*it = p;
	if(p)
		BINTREE_L(p) = r;

	return n;
}

BINTREE_FN void BINTREE_ID(sort)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_ID(cmp_t) cmpfn
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		)
{
	BINTREE_DATA *n;
	BINTREE_DATA *d; /* destination node for insertion */
	BINTREE_DATA *s; /* source node */
	BINTREE_DATA *old = *root; /* deconstruction iterator */
	int cmp;

	*root = BINTREE_NULL;

	/* we don't care about additional information like size and balancing factor to be correct here, we just need the structure of the tree to deconstruct it */
	while(old) {
		/* take one element from source tree */
		s = BINTREE_CALL(decon, &old);

		/* perform upper search */
		d = BINTREE_NULL;
		for(n = *root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, s);
			if(cmp > 0) {
				d = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}

#ifndef BINTREE_USE_BZERO
		/* otherwise, if BINTREE_USE_BZERO is defined, insert() will zero the node, so we don't have to do it here */
		bzero(BINTREE_TONODE(s), sizeof(*BINTREE_TONODE(s)));
#endif
		BINTREE_CALL(insert, root, d, s);
	}
}

BINTREE_FN int BINTREE_ID(sort_abortable)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_ID(cmp_t) cmpfn,
		int abortval /* if comparison function returns 'abortval', append remaining elements to new tree and return 'abortval'. */
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		)
{
	BINTREE_DATA *n;
	BINTREE_DATA *d; /* destination node for insertion */
	BINTREE_DATA *s; /* source node */
	BINTREE_DATA *old = *root; /* deconstruction iterator */
	int cmp;

	*root = BINTREE_NULL;

	/* we don't care about additional information like size and balancing factor to be correct here, we just need the structure of the tree to deconstruct it */
	while(old) {
		/* take one element from source tree */
		s = BINTREE_CALL(decon, &old);
#ifndef BINTREE_USE_BZERO
		/* otherwise, if BINTREE_USE_BZERO is defined, insert() will zero the node, so we don't have to do it here */
		bzero(BINTREE_TONODE(s), sizeof(*BINTREE_TONODE(s)));
#endif

		/* perform upper search */
		d = BINTREE_NULL;
		for(n = *root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, s);
			if(cmp == abortval) {
				BINTREE_CALL(insert, root, NULL, s);
				goto abort;
			}
			else if(cmp > 0) {
				d = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}

		BINTREE_CALL(insert, root, d, s);
	}
	return 0;

abort:
	while(old) {
		s = BINTREE_CALL(decon, &old);
#ifndef BINTREE_USE_BZERO
		/* otherwise, if BINTREE_USE_BZERO is defined, insert() will zero the node, so we don't have to do it here */
		bzero(BINTREE_TONODE(s), sizeof(*BINTREE_TONODE(s)));
#endif
		BINTREE_CALL(insert, root, NULL, s);
	}
	return abortval;
}
#endif

#ifdef BINTREE_USE_INDEX
BINTREE_FN BINTREE_INDEX BINTREE_ID(size)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	if(n)
		return BINTREE_SIZE(n);
	else
		return 0;
}

BINTREE_FN BINTREE_DATA *BINTREE_ID(at)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n,
		BINTREE_INDEX index)
{
	BINTREE_INDEX lsz;

	while(n) {
		if(BINTREE_L(n))
			lsz = BINTREE_SIZE(BINTREE_L(n));
		else
			lsz = 0;

		if(lsz == index)
			return n;
		else if(index < lsz)
			n = BINTREE_L(n);
		else {
			index -= lsz + 1;
			n = BINTREE_R(n);
		}
	}
	return BINTREE_NULL;
}

BINTREE_FN int BINTREE_ID(queryidx) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		const void *query,
		BINTREE_INDEX *lret,
		BINTREE_INDEX *uret,
		BINTREE_ID(qcmp_t) cmpfn
#ifdef BINTREE_USE_QCMPARG
		, BINTREE_QCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_INDEX i;
	BINTREE_INDEX s = BINTREE_CALL(size, root);
	BINTREE_INDEX c;

	if(!root) {
		if(lret)
			*lret = 0;
		if(uret)
			*uret = 0;
		return 0;
	}

	/* perform lower search */
	if(lret || !uret) {
		c = s;
		i = BINTREE_CALL(size, BINTREE_L(root));
		for(n = root;;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = i;
				n = BINTREE_L(n);
				if(!n)
					break;
				i -= BINTREE_CALL(size, BINTREE_R(n)) + 1;
			}
			else {
				n = BINTREE_R(n);
				if(!n)
					break;
				i += BINTREE_CALL(size, BINTREE_L(n)) + 1;
			}
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = s;
		i = BINTREE_CALL(size, BINTREE_L(root));
		for(n = root;;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp > 0) {
				c = i;
				n = BINTREE_L(n);
				if(!n)
					break;
				i -= BINTREE_CALL(size, BINTREE_R(n)) + 1;
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
				if(!n)
					break;
				i += BINTREE_CALL(size, BINTREE_L(n)) + 1;
			}
		}
		*uret = c;
	}
	return ret;
}

BINTREE_FN int BINTREE_ID(findidx) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		BINTREE_DATA *data,
		BINTREE_INDEX *lret,
		BINTREE_INDEX *uret,
		BINTREE_ID(cmp_t) cmpfn
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_INDEX i;
	BINTREE_INDEX s = BINTREE_CALL(size, root);
	BINTREE_INDEX c;

	if(!root) {
		if(lret)
			*lret = 0;
		if(uret)
			*uret = 0;
		return 0;
	}

	/* perform lower search */
	if(lret || !uret) {
		c = s;
		i = BINTREE_CALL(size, BINTREE_L(root));
		for(n = root;;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = i;
				n = BINTREE_L(n);
				if(!n)
					break;
				i -= BINTREE_CALL(size, BINTREE_R(n)) + 1;
			}
			else {
				n = BINTREE_R(n);
				if(!n)
					break;
				i += BINTREE_CALL(size, BINTREE_L(n)) + 1;
			}
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = s;
		i = BINTREE_CALL(size, BINTREE_L(root));
		for(n = root;;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp > 0) {
				c = i;
				n = BINTREE_L(n);
				if(!n)
					break;
				i -= BINTREE_CALL(size, BINTREE_R(n)) + 1;
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
				if(!n)
					break;
				i += BINTREE_CALL(size, BINTREE_L(n)) + 1;
			}
		}
		*uret = c;
	}
	return ret;
}

#ifdef BINTREE_USE_PARENT
BINTREE_FN BINTREE_INDEX BINTREE_ID(index)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	const BINTREE_DATA *c;
	BINTREE_INDEX idx;
	if(BINTREE_L(n))
		idx = BINTREE_SIZE(BINTREE_L(n));
	else
		idx = 0;
	while(BINTREE_P(n)) {
		c = n;
		n = BINTREE_P(n);
		if(BINTREE_R(n) == c) {
			idx++;
			if(BINTREE_L(n))
				idx += BINTREE_SIZE(BINTREE_L(n));
		}
	}
	return idx;
}
#endif
#endif

#ifdef BINTREE_USE_SUM
BINTREE_FN void BINTREE_ID(update)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	BINTREE_SUM d = BINTREE_CUMUL(n) - BINTREE_VALUE(n);
	/* old value of n: cumul(n) - cumul(l) - cumul(r) */
	if(BINTREE_L(n))
		d -= BINTREE_CUMUL(BINTREE_L(n));
	if(BINTREE_R(n))
		d -= BINTREE_CUMUL(BINTREE_R(n));
	/* d now contains old_value - new_value */
	for(BINTREE_DATA *c = n; c; c = BINTREE_P(c))
		BINTREE_CUMUL(c) -= d;
}

BINTREE_FN BINTREE_SUM BINTREE_ID(presum)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	const BINTREE_DATA *c;
	BINTREE_SUM sum;
	if(BINTREE_L(n))
		sum = BINTREE_CUMUL(BINTREE_L(n));
	else
		sum = 0;
	while(BINTREE_P(n)) {
		c = n;
		n = BINTREE_P(n);
		if(BINTREE_R(n) == c) {
			sum += BINTREE_VALUE(n);
			if(BINTREE_L(n))
				sum += BINTREE_CUMUL(BINTREE_L(n));
		}
	}
	return sum;
}

BINTREE_FN BINTREE_SUM BINTREE_ID(postsum)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n)
{
	const BINTREE_DATA *c;
	BINTREE_SUM sum;
	if(BINTREE_R(n))
		sum = BINTREE_CUMUL(BINTREE_R(n));
	else
		sum = 0;
	while(BINTREE_P(n)) {
		c = n;
		n = BINTREE_P(n);
		if(BINTREE_L(n) == c) {
			sum += BINTREE_VALUE(n);
			if(BINTREE_R(n))
				sum += BINTREE_CUMUL(BINTREE_R(n));
		}
	}
	return sum;
}

BINTREE_FN BINTREE_SUM BINTREE_ID(sum)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root)
{
	if(root)
		return BINTREE_CUMUL(root);
	else
		return 0;
}

/* assumes that values at all nodes are >= 0
 * return:
 * - off == total sum: return true
 *   - lret: leftmost node which holds presum+value == off (i.e. all following nodes have a value of 0)
 *   - uret: NULL
 * - off < total sum: return true
 *   - lret: leftmost node which holds presum+value >= off
 *   - uret: rightmost node which holds presum <= off
 * - off > total sum: return false
 * loff and uoff are only valid if lret/uret is non-NULL
 * if a chunk buffer is controlled by the tree, one could think of l begin the position to "write to" and u being the position to "read from" */
BINTREE_FN int BINTREE_ID(findoff)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		BINTREE_SUM off,
		BINTREE_DATA **lret,
		BINTREE_SUM *loff,
		BINTREE_DATA **uret,
		BINTREE_SUM *uoff)
{
	BINTREE_DATA *n;
	BINTREE_SUM lsum;

	if(lret || !uret) {
		BINTREE_SUM o = off;
		n = root;
		while(n) {
			if(BINTREE_L(n))
				lsum = BINTREE_CUMUL(BINTREE_L(n));
			else
				lsum = 0;

			if(o <= lsum && BINTREE_L(n))
				n = BINTREE_L(n);
			else if(o > lsum + BINTREE_VALUE(n)) {
				o -= lsum + BINTREE_VALUE(n);
				n = BINTREE_R(n);
			}
			else {
				if(loff)
					*loff = o - lsum;
				break;
			}
		}
		if(lret)
			*lret = n;
	}
	if(uret) {
		BINTREE_SUM o = off;
		n = root;
		while(n) {
			if(BINTREE_L(n))
				lsum = BINTREE_CUMUL(BINTREE_L(n));
			else
				lsum = 0;

			if(o < lsum)
				n = BINTREE_L(n);
			else if(o >= lsum + BINTREE_VALUE(n)) {
				o -= lsum + BINTREE_VALUE(n);
				n = BINTREE_R(n);
			}
			else {
				if(uoff)
					*uoff = o - lsum;
				break;
			}
		}
		*uret = n;
	}
	return !off || (root && off <= BINTREE_CUMUL(root));
}
#endif


/* CONST IMPLEMENTATION */

/* NOTE: query() and find() are very similar, the only difference is the cmp function for convenience and type safety */
BINTREE_FN int BINTREE_ID(cquery) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *root,
		const void *query,
		const BINTREE_DATA **lret,
		const BINTREE_DATA **uret,
		BINTREE_ID(qcmp_t) cmpfn
#ifdef BINTREE_USE_QCMPARG
		, BINTREE_QCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	const BINTREE_DATA *n;
	const BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_QCMP(cmpfn, n, query);
			if(cmp > 0) {
				c = n;
				n = BINTREE_L(n);
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
			}
		}
		*uret = c;
	}
	return ret;
}

BINTREE_FN int BINTREE_ID(cfind) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *root,
		const BINTREE_DATA *data,
		const BINTREE_DATA **lret,
		const BINTREE_DATA **uret,
		BINTREE_ID(cmp_t) cmpfn
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		)
{
	int cmp;
	int ret = 0;
	const BINTREE_DATA *n;
	const BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}
		if(lret)
			*lret = c;
	}
	/* perform upper search */
	if(uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = BINTREE_FCMP(cmpfn, n, data);
			if(cmp > 0) {
				c = n;
				n = BINTREE_L(n);
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_R(n);
			}
		}
		*uret = c;
	}
	return ret;
}

BINTREE_FN const BINTREE_DATA *BINTREE_ID(cfirst)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_L(n); n = BINTREE_L(n));
	return n;
}

BINTREE_FN const BINTREE_DATA *BINTREE_ID(clast)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_R(n); n = BINTREE_R(n));
	return n;
}

#ifdef BINTREE_USE_PARENT
BINTREE_FN const BINTREE_DATA *BINTREE_ID(cnext)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	if(BINTREE_R(n))
		for(n = BINTREE_R(n); BINTREE_L(n); n = BINTREE_L(n));
	else {
		for(; BINTREE_P(n) && n == BINTREE_R(BINTREE_P(n)); n = BINTREE_P(n));
		n = BINTREE_P(n);
	}
	return n;
}

BINTREE_FN const BINTREE_DATA *BINTREE_ID(cprev)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n)
{
	if(BINTREE_L(n))
		for(n = BINTREE_L(n); BINTREE_R(n); n = BINTREE_R(n));
	else {
		for(; BINTREE_P(n) && n == BINTREE_L(BINTREE_P(n)); n = BINTREE_P(n));
		n = BINTREE_TONODE(n)->p;
	}
	return n;
}
#endif

#ifdef BINTREE_USE_INDEX
BINTREE_FN const BINTREE_DATA *BINTREE_ID(cat)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n,
		BINTREE_INDEX index)
{
	BINTREE_INDEX lsz;

	while(n) {
		if(BINTREE_L(n))
			lsz = BINTREE_SIZE(BINTREE_L(n));
		else
			lsz = 0;

		if(lsz == index)
			return n;
		else if(index < lsz)
			n = BINTREE_L(n);
		else {
			index -= lsz + 1;
			n = BINTREE_R(n);
		}
	}
	return BINTREE_NULL;
}
#endif

#undef BINTREE_DATA
#undef BINTREE_PREFIX
#undef BINTREE_FIELD
#undef BINTREE_INDEX
#undef BINTREE_SUM
#undef BINTREE_CMPARG
#undef BINTREE_USE_PARENT
#undef BINTREE_USE_INDEX
#undef BINTREE_USE_SUM
#undef BINTREE_USE_AVL
#undef BINTREE_USE_BZERO
#undef BINTREE_USE_MULTI
#undef BINTREE_USE_QCMPARG
#undef BINTREE_USE_FCMPARG
#undef BINTREE_QCMPARG
#undef BINTREE_FCMPARG
#undef BINTREE_QCMP
#undef BINTREE_FCMP
#undef BINTREE_CONFIG
#undef BINTREE_MULTI
#undef BINTREE_TONODE
#undef BINTREE_L
#undef BINTREE_R
#undef BINTREE_P
#undef BINTREE_B
#undef BINTREE_SIZE
#undef BINTREE_CUMUL
#undef BINTREE_VALUE
#undef BINTREE_FN
#undef BINTREE_CALL
#endif

