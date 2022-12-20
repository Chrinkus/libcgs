/* cgs_bst.h
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

#include <stddef.h>

#include "cgs_variant.h"
#include "cgs_defs.h"

/**
 * Binary Search Tree
 *
 * This data structure is a step towards a Red-Black Tree. The goal here is to
 * create a structure that is reasonably generic.
 *
 * The desired operations are:
 * - new
 * - free
 * - search
 * - insert
 * - delete
 * - min
 * - max
 * - next
 * - prev
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_bst
 *
 * Forward declaration of struct defined in private header.
 */
struct cgs_bst_node;

/**
 * struct cgs_bst
 *
 * The BST container struct.
 *
 * @member root         The root node of the tree.
 * @member length       The number of elements in the tree.
 * @member cmp          A comparison function to order the tree with.
 */
struct cgs_bst {
        struct cgs_bst_node* root;
        size_t length;
        CgsCmp3Way cmp;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_bst_new
 *
 * Create a new binary search tree.
 *
 * @param cmp   The comparison function to order the tree with.
 *
 * @return      An empty bst struct.
 */
struct cgs_bst
cgs_bst_new(CgsCmp3Way cmp);

/**
 * cgs_bst_free
 *
 * Deallocate the nodes of a binary search tree.
 *
 * @param tree  Tree to be freed. May be NULL.
 */
void
cgs_bst_free(struct cgs_bst* tree);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_bst_length
 *
 * Get the length of the tree.
 *
 * @param tree  A read-only pointer to a binary search tree.
 *
 * @return      The length of the tree.
 */
inline size_t
cgs_bst_length(const struct cgs_bst* tree)
{
        return tree->length;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_bst_insert
 *
 * Allocate a new node and insert it into the tree. If equivalent node already
 * exists do nothing.
 *
 * @param tree	Tree to insert node into.
 * @param val	Value to insert into tree.
 *
 * @return	A valid pointer if successful, NULL on failure.
 */
const void*
cgs_bst_insert(struct cgs_bst* tree, struct cgs_variant* data);

/**
 * cgs_bst_search
 *
 * Search the tree for an entry. Uses the 3-way comparison function of the
 * tree to find a matching variant.
 *
 * @param tree  Tree to search.
 * @param tree  A read-only pointer to a binary search tree.
 * @param data  Data to find in the tree.
 *
 * @return      A pointer to the variant that matches the search data or NULL
 *              if not found.
 */
const struct cgs_variant*
cgs_bst_search(const struct cgs_bst* tree, const struct cgs_variant* data);

/**
 * cgs_bst_min
 *
 * Get the minimum value in the tree.
 *
 * @param tree  A read-only pointer to a binary search tree.
 *
 * @return      A read-only pointer directly to the data of the minimum
 *              element or NULL if tree is empty.
 */
const void*
cgs_bst_min(const struct cgs_bst* tree);

/**
 * cgs_bst_max
 *
 * Get the maximum value in the tree.
 *
 * @param tree  A read-only pointer to a binary search tree.
 *
 * @return      A read-only pointer directly to the data of the maximum
 *              element or NULL if tree is empty.
 */
const void*
cgs_bst_max(const struct cgs_bst* tree);

