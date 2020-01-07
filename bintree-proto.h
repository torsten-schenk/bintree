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


BINTREE_FN void BINTREE_ID(bzero) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n
);

BINTREE_FN int BINTREE_ID(memberof) (
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *root,
		const BINTREE_DATA *n
);

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
		);

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
		);

BINTREE_FN BINTREE_DATA *BINTREE_ID(first)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

BINTREE_FN BINTREE_DATA *BINTREE_ID(last)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

#ifdef BINTREE_USE_PARENT
BINTREE_FN BINTREE_DATA *BINTREE_ID(root)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

BINTREE_FN BINTREE_DATA *BINTREE_ID(next)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

BINTREE_FN BINTREE_DATA *BINTREE_ID(prev)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

BINTREE_FN void BINTREE_ID(ror)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *n);

BINTREE_FN void BINTREE_ID(rol)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *n);

/* insert 'n' node before 'p' */
BINTREE_FN void BINTREE_ID(insert)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *p,
		BINTREE_DATA *n);

BINTREE_FN void BINTREE_ID(remove)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_DATA *x);

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
		BINTREE_DATA **it);

BINTREE_FN void BINTREE_ID(sort)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA **root,
		BINTREE_ID(cmp_t) cmpfn
#ifdef BINTREE_USE_FCMPARG
		, BINTREE_FCMPARG cmparg
#endif
		);

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
		);
#endif

#ifdef BINTREE_USE_INDEX
BINTREE_FN BINTREE_INDEX BINTREE_ID(size)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);

BINTREE_FN BINTREE_DATA *BINTREE_ID(at)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n,
		BINTREE_INDEX index);

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
		);

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
		);

#ifdef BINTREE_USE_PARENT
BINTREE_FN BINTREE_INDEX BINTREE_ID(index)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);
#endif
#endif

#ifdef BINTREE_USE_SUM
BINTREE_FN void BINTREE_ID(update)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n);

/* returns:
 * - pre: sum of all element values before 'n'
 * - post: sum of all elements values after 'n' */
BINTREE_FN void BINTREE_ID(sum)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *n,
		BINTREE_SUM *pre,
		BINTREE_SUM *post);

BINTREE_FN BINTREE_SUM BINTREE_ID(total)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root);

BINTREE_FN void BINTREE_ID(findoff)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		BINTREE_DATA *root,
		BINTREE_SUM off,
		BINTREE_DATA **lret,
		BINTREE_SUM *loff,
		BINTREE_DATA **uret,
		BINTREE_SUM *uoff);
#endif

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
		);

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
		);

BINTREE_FN const BINTREE_DATA *BINTREE_ID(cfirst)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);

BINTREE_FN const BINTREE_DATA *BINTREE_ID(clast)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);

#ifdef BINTREE_USE_PARENT
BINTREE_FN const BINTREE_DATA *BINTREE_ID(cnext)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);

BINTREE_FN const BINTREE_DATA *BINTREE_ID(cprev)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n);
#endif

#ifdef BINTREE_USE_INDEX
BINTREE_FN const BINTREE_DATA *BINTREE_ID(cat)(
#ifdef BINTREE_USE_MULTI
		BINTREE_MULTI multi,
#endif
		const BINTREE_DATA *n,
		BINTREE_INDEX index);
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
#endif

