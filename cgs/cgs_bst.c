#include <stdlib.h>
#include <string.h>

#include "cgs_bst.h"
#include "cgs_variant.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Structs
struct cgs_bst {
	struct cgs_bst_node* root;
	size_t size;
	cgs_bst_cmp cmp;
};

struct cgs_bst_node {
	struct cgs_variant data;
	struct cgs_bst_node* parent;
	struct cgs_bst_node* left;
	struct cgs_bst_node* right;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Node functions - private
static struct cgs_bst_node*
cgs_bst_node_new(struct cgs_variant* data)
{
	struct cgs_bst_node* node = malloc(sizeof(struct cgs_bst_node));
	if (node) {
		memcpy(&node->data, data, sizeof(struct cgs_variant));
		node->parent = NULL;
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
	struct cgs_bst_node* node = cgs_bst_node_new(data);
	struct cgs_bst_node* parent = NULL;

	int cmp = 0;
	for (struct cgs_bst_node* temp = tree->root; temp; ) {
		parent = temp;
		const void* a = cgs_variant_get(&node->data);
		const void* b = cgs_variant_get(&temp->data);
		temp = (cmp = tree->cmp(a, b)) < 0 ? temp->left : temp->right;
	}
	node->parent = parent;
	if (!parent)
		tree->root = node;
	else if (cmp < 0)
		parent->left = node;
	else
		parent->right = node;

	++tree->size;

	return node;
}

const struct cgs_variant*
cgs_bst_search(const struct cgs_bst* tree, const struct cgs_variant* data)
{
	const struct cgs_bst_node* node = tree->root;
	for (int rc; node; ) {
		const void* a = cgs_variant_get(data);
		const void* b = cgs_variant_get(&node->data);
		rc = tree->cmp(a, b);
		if (rc == 0)
			return &node->data;
		node = rc < 0 ? node->left : node->right;
	}
	return NULL;
}

const void*
cgs_bst_min(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node && node->left)
		node = node->left;
	return node ? cgs_variant_get(&node->data) : NULL;
}

const void*
cgs_bst_max(const struct cgs_bst* tree)
{
	const struct cgs_bst_node* node = tree->root;

	while (node && node->right)
		node = node->right;
	return node ? cgs_variant_get(&node->data) : NULL;
}

