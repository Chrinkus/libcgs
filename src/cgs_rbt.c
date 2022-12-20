/* cgs_rbt.c
 *
 * MIT License
 * 
 * Copyright (c) 2022 Chris Schick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include "cgs_compare.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Node Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_rbt_node*
cgs_rbt_node_new(const struct cgs_variant* data)
{
        struct cgs_rbt_node* node = malloc(sizeof(struct cgs_rbt_node));
        if (!node)
                return NULL;

        memcpy(&node->data, data, sizeof(struct cgs_variant));
        node->color = CGS_RBT_RED;
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        return node;
}

void
cgs_rbt_node_free(struct cgs_rbt_node* node)
{
        if (!node)
                return;

        cgs_rbt_node_free(node->left);
        cgs_rbt_node_free(node->right);
        cgs_variant_free_data(&node->data);
        if (node->parent) {
                if (node == node->parent->left)
                        node->parent->left = NULL;
                else
                        node->parent->right = NULL;
        }
        free(node);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Balancing Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_rbt_rotate_left
 *
 * Swap a node's tree-position with it's right child. Rotate elements to
 * preserve binary-search-tree property.
 *
 * Warning: Only call-able under conditions where the "child" is guaranteed
 * to exist.
 *
 * @param tree  A pointer to the tree in case the rotation alters the root.
 * @param node  The node to rotate "left".
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
 * Swap a node's tree-position with it's left child. Rotate elements to
 * preserve binary-search-tree property.
 *
 * Warning: Only call-able under conditions where the "child" is guaranteed
 * to exist.
 *
 * @param tree  A pointer to the tree in case the rotation alters the root.
 * @param node  The node to rotate "right".
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
 *
 * @param tree  The tree to rebalance.
 * @param node  A pointer to a node within the tree at the point of alteration.
 */
static void
cgs_rbt_rebalance(struct cgs_rbt* tree, struct cgs_rbt_node* node)
{
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_rbt
cgs_rbt_new(CgsCmp3Way cmp)
{
        return (struct cgs_rbt){
                .root = NULL,
                .length = 0,
                .cmp = cmp,
        };
}

void
cgs_rbt_free(struct cgs_rbt* tree)
{
	if (tree)
		cgs_rbt_node_free(tree->root);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

size_t
cgs_rbt_length(const struct cgs_rbt* tree);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_rbt_insert(struct cgs_rbt* tree, struct cgs_variant* data)
{
	struct cgs_rbt_node* node = cgs_rbt_node_new(data);
        if (!node)
                return NULL;

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

	++tree->length;
	return node;
}

const struct cgs_variant*
cgs_rbt_search(const struct cgs_rbt* tree, const struct cgs_variant* data)
{
	const struct cgs_rbt_node* node = tree->root;
	const void* a = cgs_variant_get(data);

	for (int rc; node; node = rc < 0 ? node->left : node->right) {
		const void* b = cgs_variant_get(&node->data);
		rc = tree->cmp(a, b);
		if (rc == 0)
			return &node->data;
	}
	return NULL;
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Diagnostic Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const struct cgs_rbt_node*
cgs_rbt_root(const struct cgs_rbt* tree)
{
	return tree->root;
}

int
cgs_rbt_node_is_red(const struct cgs_rbt_node* node)
{
	return node ? node->color == CGS_RBT_RED : CGS_FALSE;
}

int
cgs_rbt_node_is_black(const struct cgs_rbt_node* node)
{
	return node ? node->color == CGS_RBT_BLACK : CGS_TRUE;
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

int
cgs_rbt_node_red_children_test(const struct cgs_rbt_node* node)
{
	if (!node)		// leaf node is black and has no children
		return CGS_TRUE;

	if (node->color == CGS_RBT_RED) {
		if (node->left && node->left->color == CGS_RBT_RED)
			return CGS_FALSE;
		if (node->right && node->right->color == CGS_RBT_RED)
			return CGS_FALSE;
	}

	return CGS_TRUE;	// node is black or red with black children
}

