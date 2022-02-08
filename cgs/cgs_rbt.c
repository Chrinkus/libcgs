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
	struct cgs_rbt_node* parent;
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
		node->color = CGS_RBT_RED;
		node->parent = NULL;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

static void
cgs_rbt_node_free(struct cgs_rbt_node* node)
{
	if (node) {
		cgs_rbt_node_free(node->left);
		cgs_rbt_node_free(node->right);
		cgs_variant_free_data(&node->data);
		free(node);
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions - private

static void
cgs_rbt_rotate_left(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	struct cgs_rbt_node* temp = node->right;
	node->right = temp->left;
	if (temp && temp->left)
		temp->left->parent = node;
	temp->parent = node->parent;
	if (!temp->parent)
		tree->root = temp;
	else if (node == node->parent->left)
		node->parent->left = temp;
	else
		node->parent->right = temp;
	temp->left = node;
	node->parent = temp;
}

static void
cgs_rbt_rotate_right(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	struct cgs_rbt_node* temp = node->left;
	node->left = temp->right;
	if (temp && temp->right)
		temp->right->parent = node;
	temp->parent = node->parent;
	if (!temp->parent)
		tree->root = temp;
	else if (node == node->parent->left)
		node->parent->left = temp;
	else
		node->parent->right = temp;
	temp->right = node;
	node->parent = temp;
}

static void
cgs_rbt_rebalance(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	// Fix-up tree-structure
	while (node->parent && node->parent->color == CGS_RBT_RED) {
		struct cgs_rbt_node* temp = NULL;
		if (node->parent == node->parent->parent->left) {
			// node's parent is a lefty
			temp = node->parent->parent->right;
			if (temp && temp->color == CGS_RBT_RED) {
				node->parent->color = CGS_RBT_BLACK;
				temp->color = CGS_RBT_BLACK;
				node->parent->parent->color = CGS_RBT_RED;
				node = node->parent->parent;
			} else if (node == node->parent->right) {
				node = node->parent;
				cgs_rbt_rotate_left(tree, node);
			} else {
				node->parent->color = CGS_RBT_BLACK;
				node->parent->parent->color = CGS_RBT_RED;
				cgs_rbt_rotate_right(tree, node->parent->parent);
			}
		} else {
			// node's parent is a righty
			temp = node->parent->parent->left;
			if (temp && temp->color == CGS_RBT_RED) {
				node->parent->color = CGS_RBT_BLACK;
				temp->color = CGS_RBT_BLACK;
				node->parent->parent->color = CGS_RBT_RED;
				node = node->parent->parent;
			} else if (node == node->parent->left) {
				node = node->parent;
				cgs_rbt_rotate_right(tree, node);
			} else {
				node->parent->color = CGS_RBT_BLACK;
				node->parent->parent->color = CGS_RBT_RED;
				cgs_rbt_rotate_left(tree, node->parent->parent);
			}
		}
	}
	tree->root->color = CGS_RBT_BLACK;
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
	if (tree) {
		cgs_rbt_node_free(tree->root);
		free(tree);
	}
}

size_t
cgs_rbt_size(const struct cgs_rbt* tree)
{
	return tree->size;
}

struct cgs_rbt_node*
cgs_rbt_insert(struct cgs_rbt* tree, struct cgs_variant* data)
{
	// Create new node
	struct cgs_rbt_node* node = cgs_rbt_node_new(data);

	// Walk the tree to find insertion point. Track most recent parent
	// node and result of comparison.
	struct cgs_rbt_node* parent = NULL;

	int cmp = 0;
	for (struct cgs_rbt_node* temp = tree->root; temp; ) {
		parent = temp;
		const void* a = cgs_variant_get(&node->data);
		const void* b = cgs_variant_get(&temp->data);
		temp = (cmp = tree->cmp(a, b)) < 0 ? temp->left : temp->right;
	}

	// Connect new node to its parent and parent to the node. Handle empty
	// tree case.
	node->parent = parent;
	if (!parent)
		tree->root = node;
	else if (cmp < 0)
		parent->left = node;
	else
		parent->right = node;

	// Rebalance tree
	cgs_rbt_rebalance(tree, node);

	++tree->size;
	return node;
}

const void*
cgs_rbt_min(const struct cgs_rbt* tree)
{
	const struct cgs_rbt_node* node = tree->root;

	while (node && node->left)
		node = node->left;
	return node ? cgs_variant_get(&node->data) : NULL;
}

const void*
cgs_rbt_max(const struct cgs_rbt* tree)
{
	const struct cgs_rbt_node* node = tree->root;

	while (node && node->right)
		node = node->right;
	return node ? cgs_variant_get(&node->data) : NULL;
}

