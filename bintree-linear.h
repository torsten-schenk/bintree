static inline bool binsearch(
		const void *base,
		size_t nitems,
		size_t size,
		const void *search,
		size_t *lret,
		size_t *uret,
		int (*cmpfn)(const void *a, const void *b))
{
	size_t l;
	size_t m;
	size_t u;
	int cmp;
	bool ret = false;
	const void *cur;
	size_t cand;

	if(lret || !uret) {
		l = 0;
		u = nitems;
		cand = 0;
		while(l < u) {
			m = l + (u - l) / 2;
			cur = (const char*)base + m * size;
			cmp = cmpfn(cur, search);
			if(cmp >= 0) {
				if(!cmp)
					ret = true;
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
					ret = true;
				l = m + 1;
			}
		}
		if(uret)
			*uret = cand;
	}
	return ret;
}
