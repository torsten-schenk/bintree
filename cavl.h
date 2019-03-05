#pragma once

#ifdef CAVL_PTRB
#error incomplete implementation
#else
#define CAVL_HEADER \
	void *cavl_p; \
	void *cavl_l; \
	void *cavl_r; \
	int cavl_b;
#endif

typedef struct cavl_node cavl_node_t;

struct cavl_node {
	CAVL_HEADER
};

enum {
	CAVL_ROL = 1,
	CAVL_ROR
};

typedef int (*cavl_cmp_t)(const void *a, const void *b);
typedef int (*cavl_rot_t)(int rot, void *p, void *n, void *c); /* 'rot': one of CAVL_ROL or CAVL_ROR; called after a rotation operation: pre-rotation parent, node and child are handed over as arguments */
typedef int (*cavl_cmpex_t)(const void *a, const void *b, void *user);
typedef int (*cavl_rotex_t)(int rot, void *p, void *n, void *c, void *user); /* 'rot': one of CAVL_ROL or CAVL_ROR; called after a rotation operation: pre-rotation parent, node and child are handed over as arguments */

static inline int cavl_query(
		void *root,
		const void *query, /* guaranteed to be 2nd argument of cmp function */
		void **l,
		void **u,
		cavl_cmp_t cmp)
{}

static void _cavl_ror(
		void **root,
		cavl_node_t *n)
{
	cavl_node_t *p = n->cavl_p;
	cavl_node_t *r = n->cavl_r;
	int hl = 0; /* height of left subtree (initially: n->l) */
	int hm = 0; /* height of middle subtree (initially: n->r) */
	int hr = 0; /* height of right subtree (initially: p->r) */


}

static void _cavl_rol(
		void **root,
		cavl_node_t *n)
{
	
}

/* insert 'n' node before 'p' */
static inline void cavl_insert(
		void **root,
		void *p_,
		void *n_)
{
	cavl_node_t *cur;
	cavl_node_t *n = n_;
	cavl_node_t *p = p_;
#ifdef CAVL_BZERO
	memset(n, 0, sizeof(cavl_node_t));
#endif
	if(!p) { /* append at end */
		if(*root) {
			p = *root;
			for(cur = p; cur->cavl_r; cur = cur->cavl_r);
			cur->cavl_r = n;
			n->cavl_p = cur;
		}
		else
			*root = n;
	}
	else if(p->cavl_l) { /* insert before p: rightmost position of left subtree of p */
		for(cur = p->cavl_l; cur->cavl_r; cur = cur->cavl_r);
		cur->cavl_r = n;
		n->cavl_p = cur;
	}
	else { /* insert before p: set left child */
		p->cavl_l = n;
		n->cavl_p = p;
	}

	/* retracing after insertion */
	for(p = n->cavl_p; p; n = p, p = n->cavl_p) {
		if(n == p->cavl_l) {
			if(p->cavl_b > 0) {
				p->cavl_b++;
				if(n->cavl_b < 0) {
					_cavl_rol(root, n->cavl_r);
					_cavl_ror(root, n->cavl_p);
				}
				else
					_cavl_ror(root, n);
				break;
			}
			else if(p->cavl_b < 0) {
				p->cavl_b++;
				break;
			}
			else
				p->cavl_b++;
		}
		else {
			if(p->cavl_b < 0) {
				p->cavl_b--;
				if(n->cavl_b > 0) {
					_cavl_ror(root, n->cavl_l);
					_cavl_rol(root, n->cavl_p);
				}
				else
					_cavl_rol(root, n);
				break;
			}
			else if(p->cavl_b > 0) {
				p->cavl_b--;
				break;
			}
			else
				p->cavl_b--;
		}
	}
}

static inline void cavl_remove(
		void **root,
		void *n)
{}

static inline void cavl_sort(
		void **root,
		cavl_cmp_t cmp)
{}

#if 0
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

