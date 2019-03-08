#pragma once

#ifndef BINTREE_PREFIX
#define BINTREE_PREFIX bintree
#endif

#define BINTREE_CONCAT(A, B) A ## _ ## B
#define BINTREE_CONCAT2(A, B) BINTREE_CONCAT(A, B)
#define BINTREE_ID(ID) BINTREE_CONCAT2(BINTREE_PREFIX, ID)

typedef struct BINTREE_ID(node) BINTREE_ID(node_t);

struct BINTREE_ID(node) {

};
