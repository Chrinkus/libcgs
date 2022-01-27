#include <stdlib.h>

#include "cgs_bst.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Enums

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Structs
struct cgs_bst {
	struct cgs_bst_node* root;
	size_t size;
};

struct cgs_bst_node {
	int key;
	struct cgs_bst_node* left;
	struct cgs_bst_node* right;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Node functions
static struct cgs_bst_node* cgs_bst_node_new(int val)
{
	struct cgs_bst_node* node = malloc(sizeof(struct cgs_bst_node));
	if (node) {
		node->key = val;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

static void cgs_bst_node_free(struct cgs_bst_node* node)
{
	if (node) {
		cgs_bst_node_free(node->left);
		cgs_bst_node_free(node->right);
	}
	free(node);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions
struct cgs_bst* cgs_bst_new(void)
{
	struct cgs_bst* tree = malloc(sizeof(struct cgs_bst));
	if (tree) {
		tree->root = NULL;
		tree->size = 0;
	}
	return tree;
}

void cgs_bst_free(struct cgs_bst* tree)
{
	if (tree)
		cgs_bst_node_free(tree->root);
	free(tree);
}

