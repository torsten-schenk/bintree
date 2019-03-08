#ifndef BINTREE_CONFIG
#error missing BINTREE_CONFIG before include statement
#else

#define BINTREE_PREFIX BINTREE_SCONCAT2(BINTREE_CONFIG, _PREFIX)
#define BINTREE_FIELD BINTREE_SCONCAT2(BINTREE_CONFIG, _FIELD)
#define BINTREE_DATA BINTREE_SCONCAT2(BINTREE_CONFIG, _DATA)

#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_PARENT) != 0
#define BINTREE_USE_PARENT
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_INDEX) != 0
#define BINTREE_USE_INDEX
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_AVL) != 0
#define BINTREE_USE_AVL
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_BZERO) != 0
#define BINTREE_USE_BZERO
#endif

typedef int (*BINTREE_ID(cmp_t))(const BINTREE_DATA *a, const BINTREE_DATA *b);
typedef int (*BINTREE_ID(qcmp_t))(const BINTREE_DATA *a, const void *b);

/* NOTE: query() and find() are very similar, the only difference is the cmp function for convenience and type safety */
static inline int BINTREE_ID(query) (
		BINTREE_DATA *root,
		const void *query,
		BINTREE_DATA **lret,
		BINTREE_DATA **uret,
		BINTREE_ID(qcmp_t) cmpfn)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, query);
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
			cmp = cmpfn(n, query);
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

static inline int BINTREE_ID(find) (
		BINTREE_DATA *root,
		const BINTREE_DATA *data,
		BINTREE_DATA **lret,
		BINTREE_DATA **uret,
		BINTREE_ID(cmp_t) cmpfn)
{
	int cmp;
	int ret = 0;
	BINTREE_DATA *n;
	BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, data);
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
			cmp = cmpfn(n, data);
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

static inline BINTREE_DATA *BINTREE_ID(first)(
		BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_L(n); n = BINTREE_L(n));
	return n;
}

static inline BINTREE_DATA *BINTREE_ID(last)(
		BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_R(n); n = BINTREE_R(n));
	return n;
}

#ifdef BINTREE_USE_PARENT
static inline BINTREE_DATA *BINTREE_ID(next)(
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

static inline BINTREE_DATA *BINTREE_ID(prev)(
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

static inline void BINTREE_ID(ror)(
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
	size_t sl;
	size_t sm;
	size_t sr;
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

static inline void BINTREE_ID(rol)(
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
	size_t sl;
	size_t sm;
	size_t sr;
#endif

#ifdef BINTREE_USE_INDEX
	/* calculate sizes of the three subtrees and set the new sizue for p and n */
	if(BINTREE_R(n))
		sr = BINTREE_SIZE(BINTREE_R(n));
	else
		sr = 0;
	sm = BINTREE_SIZE(n) - sr - 1;
	sl = BINTREE_SIZE(p) - sm - sr - 2;
	BINTREE_SIZE(n) = sl + sm + sr + 2;
	BINTREE_SIZE(p) = sl + sm + 1; 
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
static inline void BINTREE_ID(insert)(
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

#ifdef BINTREE_USE_AVL
	/* retracing after insertion */
	for(p = BINTREE_P(n); p; n = p, p = BINTREE_P(n)) {
		if(n == BINTREE_L(p)) {
			if(!BINTREE_B(p)) /* b = 0 */
				BINTREE_B(p)++;
			else if(BINTREE_B(p) > 0) { /* b = 1 */
				BINTREE_B(p)++;
				if(BINTREE_B(n) < 0) { /* b = -1 */
					BINTREE_ID(rol)(root, BINTREE_R(n));
					BINTREE_ID(ror)(root, BINTREE_P(n));
				}
				else
					BINTREE_ID(ror)(root, n);
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
					BINTREE_ID(ror)(root, BINTREE_L(n));
					BINTREE_ID(rol)(root, BINTREE_P(n));
				}
				else
					BINTREE_ID(rol)(root, n);
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

static inline void BINTREE_ID(remove)(
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
		y = BINTREE_L(x);
		if(BINTREE_R(y)) {
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
		else {
			zp = y;
#ifdef BINTREE_USE_AVL
			zr = 0;
#endif
		}
		BINTREE_P(y) = BINTREE_P(x);
		BINTREE_R(y) = BINTREE_R(x);
#ifdef BINTREE_USE_AVL
		BINTREE_B(y) = BINTREE_B(x);
#endif
#ifdef BINTREE_USE_INDEX
		BINTREE_SIZE(y) = BINTREE_SIZE(x);
#endif
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
#ifdef BINTREE_USE_AVL
		if(zp && BINTREE_L(zp) == x)
			zr = 0;
#endif
		if(!BINTREE_P(x)) { /* x was root node, set new root node */
			if(BINTREE_L(x)) {
				*root = BINTREE_L(x);
				BINTREE_P(BINTREE_L(x)) = BINTREE_NULL;
			}
			else if(BINTREE_R(x)) {
				*root = BINTREE_R(x);
				BINTREE_P(BINTREE_R(x)) = BINTREE_NULL;
			}
			else
				*root = BINTREE_NULL;
		}
		else if(BINTREE_L(x)) { /* x has only a left subtree and is not root, attach subtree to parent at x's position */
			if(BINTREE_L(BINTREE_P(x)) == x)
				BINTREE_L(BINTREE_P(x)) = BINTREE_L(x);
			else
				BINTREE_R(BINTREE_P(x)) = BINTREE_L(x);
			BINTREE_P(BINTREE_L(x)) = BINTREE_P(x);
		}
		else if(BINTREE_R(x)) { /* x has only a right subtree and is not root, attach subtree to parent at x's position */
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

#ifdef BINTREE_USE_AVL
	/* retracing after remove */

	/* if zr: right subtree of zp has been decreased in height
	 * otherwise: left subtree of zp has been decresed in height */
	while(zp) {
		if(zr) {
			if(!BINTREE_B(zp)) { /* case 1 */
				BINTREE_B(zp)++;
				break;
			}
			else if(BINTREE_B(zp) > 0) {
				c = BINTREE_L(zp);
				BINTREE_B(zp)++;
				if(!BINTREE_B(c)) { /* case 2 */
					BINTREE_ID(ror)(root, c);
					break;
				}
				else if(BINTREE_B(c) < 0) { /* case 3 */
					BINTREE_ID(rol)(root, BINTREE_R(c));
					BINTREE_ID(ror)(root, BINTREE_P(c));
				}
				else /* case 4 */
					BINTREE_ID(ror)(root, c);
			}
			else /* case 5 */
				BINTREE_B(zp)++;
		}
		else {
			if(!BINTREE_B(zp)) {
				BINTREE_B(zp)--;
				break;
			}
			else if(BINTREE_B(zp) < 0) {
				c = BINTREE_R(zp);
				BINTREE_B(zp)--;
				if(!BINTREE_B(c)) {
					BINTREE_ID(rol)(root, c);
					break;
				}
				else if(BINTREE_B(c) > 0) {
					BINTREE_ID(ror)(root, BINTREE_L(c));
					BINTREE_ID(rol)(root, BINTREE_P(c));
				}
				else
					BINTREE_ID(rol)(root, c);
			}
			else
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

static inline void BINTREE_ID(sort)(
		BINTREE_DATA **root,
		BINTREE_ID(cmp_t) cmpfn)
{
	BINTREE_DATA *r;
	BINTREE_DATA *p;
	BINTREE_DATA *n;
	BINTREE_DATA *d; /* destination node for insertion */
	BINTREE_DATA *s = *root; /* source node */
	int cmp;

	*root = BINTREE_NULL;

	/* we don't care about additional information like size and balancing factor to be correct here, we just need the structure of the tree to deconstruct it */
	while(s) {
		for(; BINTREE_L(s); s = BINTREE_L(s));
		p = BINTREE_P(s);
		r = BINTREE_R(s);

		/* perform upper search */
		d = BINTREE_NULL;
		for(n = *root; n;) {
			cmp = cmpfn(n, s);
			if(cmp > 0) {
				d = n;
				n = BINTREE_L(n);
			}
			else
				n = BINTREE_R(n);
		}

#ifndef BINTREE_USE_BZERO
		/* otherwise, if BINTREE_USE_BZERO is defined, insert() will zero the node, so we don't have to do it here */
		bzero(BINTREE_TONODE(s), sizeof(BINTREE_ID(node_t)));
#endif
		BINTREE_ID(insert)(root, d, s);

		/* purpose here is to put r into s's position (as left child of p) and continue with p. */
		if(r) {
			BINTREE_P(r) = p;
			s = r; /* we know that we will descend into r again in next loop iteration, so continue with r */
		}
		else
			s = p;
		if(p)
			BINTREE_L(p) = r;
	}
}
#endif

#ifdef BINTREE_USE_INDEX
static inline size_t BINTREE_ID(size)(
		const BINTREE_DATA *n)
{
	return BINTREE_SIZE(n);
}

static inline size_t BINTREE_ID(index)(
		const BINTREE_DATA *n)
{
	const BINTREE_DATA *c;
	size_t idx;
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

static inline BINTREE_DATA *BINTREE_ID(at)(
		BINTREE_DATA *n,
		size_t index)
{
	size_t lsz;

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

/* CONST IMPLEMENTATION */

/* NOTE: query() and find() are very similar, the only difference is the cmp function for convenience and type safety */
static inline int BINTREE_ID(cquery) (
		const BINTREE_DATA *root,
		const void *query,
		const BINTREE_DATA **lret,
		const BINTREE_DATA **uret,
		BINTREE_ID(qcmp_t) cmpfn)
{
	int cmp;
	int ret = 0;
	const BINTREE_DATA *n;
	const BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, query);
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
			cmp = cmpfn(n, query);
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

static inline int BINTREE_ID(cfind) (
		const BINTREE_DATA *root,
		const BINTREE_DATA *data,
		const BINTREE_DATA **lret,
		const BINTREE_DATA **uret,
		BINTREE_ID(cmp_t) cmpfn)
{
	int cmp;
	int ret = 0;
	const BINTREE_DATA *n;
	const BINTREE_DATA *c;

	/* perform lower search */
	if(lret || !uret) {
		c = BINTREE_NULL;
		for(n = root; n;) {
			cmp = cmpfn(n, data);
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
			cmp = cmpfn(n, data);
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

static inline const BINTREE_DATA *BINTREE_ID(cfirst)(
		const BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_L(n); n = BINTREE_L(n));
	return n;
}

static inline const BINTREE_DATA *BINTREE_ID(clast)(
		const BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_R(n); n = BINTREE_R(n));
	return n;
}

#ifdef BINTREE_USE_PARENT
static inline const BINTREE_DATA *BINTREE_ID(cnext)(
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

static inline const BINTREE_DATA *BINTREE_ID(cprev)(
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
static inline const BINTREE_DATA *BINTREE_ID(cat)(
		const BINTREE_DATA *n,
		size_t index)
{
	size_t lsz;

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
#undef BINTREE_USE_PARENT
#undef BINTREE_USE_INDEX
#undef BINTREE_USE_AVL
#undef BINTREE_USE_BZERO
#undef BINTREE_CONFIG
#endif

