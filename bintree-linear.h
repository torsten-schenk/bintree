/* return:
 *   0: exact match not found
 *   1: exact match found at least once */
static inline int binsearch(
		const void *base,
		size_t nitems,
		size_t size,
		const void *search,
		size_t *lret,
		size_t *uret,
		int (*cmpfn)(const void *a, const void *b)) /* 'a' is always an element in the 'base' array; 'b' is always the 'search' parameter */
{
	size_t l;
	size_t m;
	size_t u;
	int cmp;
	int ret = 0;
	const void *cur;
	size_t cand;

	if(lret || !uret) {
		l = 0;
		u = nitems;
		cand = nitems;
		while(l < u) {
			m = l + (u - l) / 2;
			cur = (const char*)base + m * size;
			cmp = cmpfn(cur, search);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				cand = m;
				u = m;
			}
			else
				l = m + 1;
		}
		if(lret)
			*lret = cand;
	}
	/* perform upper search */
	if(uret) {
		l = 0;
		u = nitems;
		cand = nitems;
		while(l < u) {
			m = l + (u - l) / 2;
			cur = (const char*)base + m * size;
			cmp = cmpfn(cur, search);
			if(cmp > 0) {
				cand = m;
				u = m;
			}
			else {
				if(!cmp)
					ret = 1;
				l = m + 1;
			}
		}
		*uret = cand;
	}
	return ret;
}

static inline int linsearch(
		const void *base,
		size_t nitems,
		size_t size,
		const void *search,
		size_t *lret,
		size_t *uret,
		int (*cmpfn)(const void *a, const void *b)) /* 'a' is always an element in the 'base' array; 'b' is always the 'search' parameter */
{
	int ret = 0;
	int cmp = 1;
	size_t l;
	size_t i;
	const char *si;

	si = base;
	for(i = 0; i < nitems; i++, si += size) {
		cmp = cmpfn(si, search);
		if(cmp >= 0)
			break;
	}
	if(lret)
		*lret = i;

	if(cmp == 0)
		ret = 1;

	if(uret) {
		if(cmp > 0)
			*uret = i;
		else {
			for(; i < nitems; i++, si += size) {
				cmp = cmpfn(si, search);
				if(cmp > 0)
					break;
			}
			*uret = i;
		}
	}
	return ret;
}

static inline int binsearch_arg(
		const void *base,
		size_t nitems,
		size_t size,
		const void *search,
		size_t *lret,
		size_t *uret,
		int (*cmpfn)(const void *a, const void *b, void *arg),
		void *arg) /* 'a' is always an element in the 'base' array; 'b' is always the 'search' parameter */
{
	size_t l;
	size_t m;
	size_t u;
	int cmp;
	int ret = 0;
	const void *cur;
	size_t cand;

	if(lret || !uret) {
		l = 0;
		u = nitems;
		cand = nitems;
		while(l < u) {
			m = l + (u - l) / 2;
			cur = (const char*)base + m * size;
			cmp = cmpfn(cur, search, arg);
			if(cmp >= 0) {
				if(!cmp)
					ret = 1;
				cand = m;
				u = m;
			}
			else
				l = m + 1;
		}
		if(lret)
			*lret = cand;
	}
	/* perform upper search */
	if(uret) {
		l = 0;
		u = nitems;
		cand = nitems;
		while(l < u) {
			m = l + (u - l) / 2;
			cur = (const char*)base + m * size;
			cmp = cmpfn(cur, search, arg);
			if(cmp > 0) {
				cand = m;
				u = m;
			}
			else {
				if(!cmp)
					ret = 1;
				l = m + 1;
			}
		}
		if(uret)
			*uret = cand;
	}
	return ret;
}

