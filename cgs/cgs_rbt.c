#include <stdlib.h>
#include <string.h>

#include "cgs_rbt.h"
#include "cgs_variant.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Enums

enum cgs_rbt_colors { CGS_RBT_RED, CGS_RBT_BLACK };

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Structure definitions

struct cgs_rbt {
	struct cgs_rbt_node* root;
	size_t size;
	cgs_rbt_cmp cmp;
};

struct cgs_rbt_node {
	struct cgs_variant data;
	int color;
	struct cgs_rbt_node* left;
	struct cgs_rbt_node* right;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Node functions - private

static struct cgs_rbt_node*
cgs_rbt_node_new(struct cgs_variant* data)
{
	struct cgs_rbt_node* node = malloc(sizeof(struct cgs_rbt_node));
	if (node) {
		memcpy(&node->data, data, sizeof(struct cgs_variant));
		node->color = CGS_RBT_BLACK;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

void
cgs_rbt_node_free(struct cgs_rbt_node* node)
{
	if (node) {
		cgs_rbt_node_free(node->left);
		cgs_rbt_node_free(node->right);
		cgs_variant_free_data(&node->data);
	}
	free(node);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions - public

struct cgs_rbt*
cgs_rbt_new(cgs_rbt_cmp cmp)
{
	struct cgs_rbt* tree = malloc(sizeof(struct cgs_rbt));
	if (tree) {
		tree->root = NULL;
		tree->size = 0;
		tree->cmp = cmp;
	}
	return tree;
}

void
cgs_rbt_free(struct cgs_rbt* tree)
{
	if (tree)
		cgs_rbt_node_free(tree->root);
	free(tree);
}

size_t
cgs_rbt_size(const struct cgs_rbt* tree)
{
	return tree->size;
}

