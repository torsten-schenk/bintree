#ifndef _BINTREE_INCLUDED
#define _BINTREE_INCLUDED
#define BINTREE_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define BINTREE_SCONCAT(A, B) A ## B
#define BINTREE_SCONCAT2(A, B) BINTREE_SCONCAT(A, B)
#define BINTREE_CONCAT(A, B) A ## _ ## B
#define BINTREE_CONCAT2(A, B) BINTREE_CONCAT(A, B)
#define BINTREE_ID(ID) BINTREE_CONCAT2(BINTREE_PREFIX, ID)
#define BINTREE_NULL ((void*)0)
#endif

#ifndef BINTREE_CONFIG
#error missing BINTREE_CONFIG before include statement
#else

#define BINTREE_FIELD BINTREE_SCONCAT2(BINTREE_CONFIG, _FIELD)
#define BINTREE_DATA BINTREE_SCONCAT2(BINTREE_CONFIG, _DATA)

#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_MULTI) != 0
#define BINTREE_USE_MULTI
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_PARENT) != 0
#define BINTREE_USE_PARENT
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_INDEX) != 0
#define BINTREE_USE_INDEX
#define BINTREE_INDEX BINTREE_SCONCAT2(BINTREE_CONFIG, _INDEX)
#endif
#if BINTREE_SCONCAT2(BINTREE_CONFIG, _USE_AVL) != 0
#define BINTREE_USE_AVL
#endif

/* if BINTREE_USE_MULTI is specified, an array of BINTREE_FIELD is expected to be present in BINTREE_DATA.
 * including this file will create the BINTREE_FIELD struct.
 * otherwise, including this file will create an anonymous field in the data struct named BINTREE_FIELD. */
#ifndef BINTREE_USE_MULTI
struct {
#endif
#ifdef BINTREE_USE_PARENT
	BINTREE_DATA *p;
#endif
	BINTREE_DATA *l;
	BINTREE_DATA *r;
#ifdef BINTREE_USE_INDEX
	BINTREE_INDEX size;
#endif
#ifdef BINTREE_USE_AVL
	int b;
#endif
#ifndef BINTREE_USE_MULTI
} BINTREE_FIELD;
#endif

#undef BINTREE_DATA
#undef BINTREE_FIELD
#undef BINTREE_INDEX
#undef BINTREE_USE_PARENT
#undef BINTREE_USE_INDEX
#undef BINTREE_USE_AVL
#undef BINTREE_USE_MULTI
#undef BINTREE_CONFIG

#endif

