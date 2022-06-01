/* cgs_bst_private.h
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
#pragma once

#include "cgs_bst.h"
#include "cgs_variant.h"
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Node Private Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_bst_node
 *
 * A data structure representing a node in a binary search tree.
 *
 * @member data         A variant containing the value of this node.
 * @member parent       The parent of this node or NULL if node is root.
 * @member left         The left child of this node or NULL if node is a leaf.
 * @member right        The right child of this node or NULL if node is a leaf.
 */
struct cgs_bst_node {
        struct cgs_variant data;
        struct cgs_bst_node* parent;
        struct cgs_bst_node* left;
        struct cgs_bst_node* right;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Node Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_bst_node_new
 *
 * Allocate a node and copy a value into it. Caller sets parent.
 *
 * @param data  A pointer to a variant containing the data for the node.
 *
 * @return      A pointer to the allocated node on success or NULL on failure.
 */
struct cgs_bst_node*
cgs_bst_node_new(const struct cgs_variant* data);

/**
 * cgs_bst_node_free
 *
 * Free the sub-trees of a node and then the node itself.
 */
void
cgs_bst_node_free(struct cgs_bst_node* node);

