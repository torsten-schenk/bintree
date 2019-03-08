#include <stdio.h>
typedef struct mydata mydata_t;

#define BINTREE_PREFIX mydata
#define BINTREE_DATA mydata_t
#define BINTREE_FIELD node
#define BINTREE_USE_PARENT
#define BINTREE_USE_INDEX
#define BINTREE_USE_AVL
#include "bintree-hdr.h"
struct mydata {
	mydata_node_t node;
};
#include "bintree-impl.h"

