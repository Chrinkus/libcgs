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
static struct cgs_bst_node*
cgs_bst_node_new(int val)
{
	struct cgs_bst_node* node = malloc(sizeof(struct cgs_bst_node));
	if (node) {
		node->key = val;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

static void
cgs_bst_node_free(struct cgs_bst_node* node)
{
	if (node) {
		cgs_bst_node_free(node->left);
		cgs_bst_node_free(node->right);
	}
	free(node);
}

static struct cgs_bst_node*
cgs_bst_node_insert(struct cgs_bst_node* node, int val)
{
	if (!node)
		node = cgs_bst_node_new(val);
	else if (val < node->key)
		node->left = cgs_bst_node_insert(node->left, val);
	else
		node->right = cgs_bst_node_insert(node->right, val);

	return node;
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

size_t cgs_bst_size(const struct cgs_bst* tree)
{
	return tree->size;
}

struct cgs_bst_node* cgs_bst_insert(struct cgs_bst* tree, int val)
{
	tree->root = cgs_bst_node_insert(tree->root, val);
	++tree->size;

	return tree->root;
}

int cgs_bst_min(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node->left)
		node = node->left;
	return node->key;
}

int cgs_bst_max(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node->right)
		node = node->right;
	return node->key;
}

