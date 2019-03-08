static inline int BINTREE_ID(query) (
		BINTREE_DATA *root,
		const void *query,
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
			cmp = cmpfn(n, query);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				c = n;
				n = BINTREE_TONODE(n)->l;
			}
			else
				n = BINTREE_TONODE(n)->r;
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
				n = BINTREE_TONODE(n)->l;
			}
			else {
				if(!cmp)
					ret = 1;
				n = BINTREE_TONODE(n)->r;
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
		for(; BINTREE_TONODE(n)->l; n = BINTREE_TONODE(n)->l);
	return n;
}

static inline BINTREE_DATA *BINTREE_ID(last)(
		BINTREE_DATA *n)
{
	if(n)
		for(; BINTREE_TONODE(n)->r; n = BINTREE_TONODE(n)->r);
	return n;
}

#ifdef BINTREE_USE_PARENT
static inline BINTREE_DATA *BINTREE_ID(next)(
		BINTREE_DATA *n)
{
	if(BINTREE_TONODE(n)->r)
		for(n = BINTREE_TONODE(n)->r; BINTREE_TONODE(n)->l; n = BINTREE_TONODE(n)->l);
	else {
		for(; BINTREE_TONODE(n)->p && n == BINTREE_TONODE(BINTREE_TONODE(n)->p)->r; n = BINTREE_TONODE(n)->p);
		n = BINTREE_TONODE(n)->p;
	}
	return n;
}

static inline BINTREE_DATA *BINTREE_ID(prev)(
		BINTREE_DATA *n)
{
	if(BINTREE_TONODE(n)->l)
		for(n = BINTREE_TONODE(n)->l; BINTREE_TONODE(n)->r; n = BINTREE_TONODE(n)->r);
	else {
		for(; BINTREE_TONODE(n)->p && n == BINTREE_TONODE(BINTREE_TONODE(n)->p)->l; n = BINTREE_TONODE(n)->p);
		n = BINTREE_TONODE(n)->p;
	}
	return n;
}
#endif

#undef BINTREE_USE_AVL
#undef BINTREE_USE_PARENT
#undef BINTREE_USE_INDEX
#undef BINTREE_DATA
#undef BINTREE_FIELD
#undef BINTREE_PREFIX

