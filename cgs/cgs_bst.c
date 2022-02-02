#include <stdlib.h>
#include <string.h>

#include "cgs_bst.h"
#include "cgs_variant.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Enums

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Structs
struct cgs_bst {
	struct cgs_bst_node* root;
	size_t size;
	cgs_bst_cmp cmp;
};

struct cgs_bst_node {
	struct cgs_variant data;
	struct cgs_bst_node* left;
	struct cgs_bst_node* right;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Node functions
static struct cgs_bst_node*
cgs_bst_node_new(struct cgs_variant* data)
{
	struct cgs_bst_node* node = malloc(sizeof(struct cgs_bst_node));
	if (node) {
		memcpy(&node->data, data, sizeof(struct cgs_variant));
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
		cgs_variant_free_data(&node->data);
	}
	free(node);
}

static struct cgs_bst_node*
cgs_bst_node_insert(struct cgs_bst_node* node, struct cgs_variant* data,
		cgs_bst_cmp cmp)
{
	if (!node)
		node = cgs_bst_node_new(data);
	else if (cmp(cgs_variant_get(data), cgs_variant_get(&node->data)) < 0)
		node->left = cgs_bst_node_insert(node->left, data, cmp);
	else
		node->right = cgs_bst_node_insert(node->right, data, cmp);

	return node;
}

static struct cgs_variant*
cgs_bst_node_search(struct cgs_bst_node* node, const struct cgs_variant* data,
		cgs_bst_cmp cmp)
{
	for (int rc; node; ) {
		rc = cmp(cgs_variant_get(data), cgs_variant_get(&node->data));
		if (rc == 0)
			return &node->data;
		else if (rc < 0)
			node = node->left;
		else
			node = node->right;
	}
	return NULL;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions
struct cgs_bst* cgs_bst_new(cgs_bst_cmp cmp)
{
	struct cgs_bst* tree = malloc(sizeof(struct cgs_bst));
	if (tree) {
		tree->root = NULL;
		tree->size = 0;
		tree->cmp = cmp;
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

struct cgs_bst_node*
cgs_bst_insert(struct cgs_bst* tree, struct cgs_variant* data)
{
	tree->root = cgs_bst_node_insert(tree->root, data, tree->cmp);
	++tree->size;

	return tree->root;
}

struct cgs_variant*
cgs_bst_search(struct cgs_bst* tree, const struct cgs_variant* data)
{
	return cgs_bst_node_search(tree->root, data, tree->cmp);
}

const void*
cgs_bst_min(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node->left)
		node = node->left;
	return cgs_variant_get(&node->data);
}

const void*
cgs_bst_max(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node->right)
		node = node->right;
	return cgs_variant_get(&node->data);
}

