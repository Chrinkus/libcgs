/* cgs_rbt.c
 *
 * Copyright (C) 2022 Chris Schick <seeschickrun@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 * cgs_rbt.c
 *
 * This file contains the source code of the libcgs implementation of a
 * red-black tree.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#include <stdlib.h>
#include <string.h>

#include "cgs_rbt.h"
#include "cgs_rbt_private.h"
#include "cgs_variant.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Node functions - private

struct cgs_rbt_node*
cgs_rbt_node_new(const struct cgs_variant* data)
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

void
cgs_rbt_node_free(struct cgs_rbt_node* node)
{
	if (node) {
		cgs_rbt_node_free(node->left);
		cgs_rbt_node_free(node->right);
		cgs_variant_free_data(&node->data);
		if (node->parent) {
			if (node == node->parent->left)
				node->parent->left = NULL;
			else
				node->parent->right = NULL;
		}
	}
	free(node);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions - private

/**
 * cgs_rbt_rotate_left
 *
 * Swap a node with it's right child. Only called under conditions where the
 * "child" is guaranteed to exist.
 */
static void
cgs_rbt_rotate_left(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	struct cgs_rbt_node* child = node->right;
	node->right = child->left;
	if (child->left)
		child->left->parent = node;
	child->parent = node->parent;
	if (!child->parent)
		tree->root = child;
	else if (node == node->parent->left)
		node->parent->left = child;
	else
		node->parent->right = child;
	child->left = node;
	node->parent = child;
}

/**
 * cgs_rbt_rotate_right
 *
 * Swap a node with it's left child. Only called under conditions where the
 * "child" is guaranteed to exist.
 */
static void
cgs_rbt_rotate_right(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	struct cgs_rbt_node* child = node->left;
	node->left = child->right;
	if (child->right)
		child->right->parent = node;
	child->parent = node->parent;
	if (!child->parent)
		tree->root = child;
	else if (node == node->parent->left)
		node->parent->left = child;
	else
		node->parent->right = child;
	child->right = node;
	node->parent = child;
}

/**
 * cgs_rbt_rebalance
 *
 * Check the tree structure after adding or deleting a node. Terminates when
 * node's parent is not RED or is NULL (root).
 *
 * - If node's parent is a lefty
 * 	- get aunty right
 * 	- If aunt exists and is RED
 * 		- node and aunt become BLACK
 * 		- grandparent becomes RED
 * 		- move node pointer to grandparent and iterate 
 * 	- Else if node is a righty
 * 		- move node pointer to parent
 * 		- rotate left and iterate
 * 	- Else node is a lefty
 * 		- node's parent becomes BLACK
 * 		- grandparent becomes RED
 * 		- rotate right and break loop (parent is BLACK)
 * 		
 * - Else node's parent is a righty
 *   	- get aunty left
 *   	- If aunt exists and is RED
 *   		- node and aunt become BLACK
 *   		- grandparent becomes RED
 *   		- move node pointer to grandparent and iterate
 *   	- Else if node is a lefty
 *   		- move node pointer to parent
 *   		- rotate right and iterate
 *   	- Else node is a righty
 *   		- node's parent becomes BLACK
 *   		- grandparent becomes RED
 *   		- rotate left and break loop (parent is BLACK)
 *
 * Double parent dereferencing may result in NULL leaves which are assumed to
 * be BLACK.
 */
static void
cgs_rbt_rebalance(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
	// Fix-up tree-structure
	while (node->parent && node->parent->color == CGS_RBT_RED) {
		struct cgs_rbt_node* aunt = NULL;
		if (node->parent == node->parent->parent->left) {
			aunt = node->parent->parent->right;
			if (aunt && aunt->color == CGS_RBT_RED) {
				node->parent->color = CGS_RBT_BLACK;
				aunt->color = CGS_RBT_BLACK;
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
			aunt = node->parent->parent->left;
			if (aunt && aunt->color == CGS_RBT_RED) {
				node->parent->color = CGS_RBT_BLACK;
				aunt->color = CGS_RBT_BLACK;
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

const struct cgs_variant*
cgs_rbt_search(const struct cgs_rbt* tree, const struct cgs_variant* data)
{
	const struct cgs_rbt_node* node = tree->root;
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Tree functions - diagnostic

const struct cgs_rbt_node*
cgs_rbt_root(const struct cgs_rbt* tree)
{
	return tree->root;
}

int
cgs_rbt_node_is_red(const struct cgs_rbt_node* node)
{
	return node ? node->color == CGS_RBT_RED : 1;
}

int
cgs_rbt_node_is_black(const struct cgs_rbt_node* node)
{
	return node ? node->color == CGS_RBT_BLACK : 1;
}

int
cgs_rbt_node_black_height(const struct cgs_rbt_node* node)
{
	if (!node)
		return 1;	// Leaf nodes are considered black

	int left = cgs_rbt_node_black_height(node->left);
	int right = cgs_rbt_node_black_height(node->right);
	if (left != right || left == -1)
		return -1;

	return left + cgs_rbt_node_is_black(node);
}

