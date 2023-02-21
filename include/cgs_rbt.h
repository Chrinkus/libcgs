/* cgs_rbt.h
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
 * cgs_rbt.h
 *
 * This file contains the public API for the libcgs implementation of a
 * red-black tree.
 *
 * Red-Black Tree
 *
 * A balanced binary-search tree.
 *
 * Operations are similar to a binary-search tree:
 * - new
 * - free
 * - insert
 * - delete
 * - search
 * - min
 * - max
 * - next
 * - prev
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#pragma once

#include <stddef.h>

#include "cgs_variant.h"
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_rbt_node
 *
 * Forward declaration of struct defined in private header.
 */
struct cgs_rbt_node;

/**
 * struct cgs_rbt
 *
 * The RBT container struct.
 *
 * @member root         The root node of the tree.
 * @member length       The number of elements in the tree.
 * @member cmp          A comparison function to order the tree with.
 * @member ff           An optional freeing function for the elements of
 *                      the tree or NULL if they are trivially deallocated.
 */
struct cgs_rbt {
        struct cgs_rbt_node* root;
        size_t length;
        CgsCmp3Way cmp;
        CgsFreeFunc ff;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Mangement Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_rbt_new
 *
 * Create a new red-black tree.
 *
 * @param cmp   The comparison function to order the tree with.
 * @param ff    An optional freeing function for the elements of the tree or
 *              NULL if they are trivially deallocated.
 *
 * @return      An empty red-black tree.
 */
struct cgs_rbt
cgs_rbt_new(CgsCmp3Way cmp, CgsFreeFunc ff);

/**
 * cgs_rbt_free
 *
 * Deallocate the nodes of a red-black tree.
 *
 * @param tree  A pointer to the tree to be freed.
 */
void
cgs_rbt_free(struct cgs_rbt* tree);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_rbt_length
 *
 * Get the number of elements in the tree.
 *
 * @param tree  A read-only pointer to a red-black tree.
 *
 * @return      The length member of the tree.
 */
inline size_t
cgs_rbt_length(const struct cgs_rbt* tree)
{
        return tree->length;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * RBT Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_rbt_insert
 *
 * Allocate a new node and insert it into the tree.
 *
 * @param tree  A pointer to a tree to insert a node into.
 * @param val   Value of the node.
 *
 * @return      A valid pointer if successful, NULL on failure.
 */
const void*
cgs_rbt_insert(struct cgs_rbt* tree, struct cgs_variant* data);

/**
 * cgs_rbt_search
 *
 * Search the tree for an entry. Uses the 3-way comparison function of the
 * tree to find a matching variant.
 *
 * @param tree  Tree to search.
 * @param data  Data to find in the tree.
 *
 * @return      A read-only pointer to the variant that matches the search
 *              data or NULL if not found.
 */
const struct cgs_variant*
cgs_rbt_search(const struct cgs_rbt* tree, const struct cgs_variant* data);

/**
 * cgs_rbt_min
 *
 * Get the minimum value in the tree.
 *
 * @param tree  Tree to find minimum value of.
 *
 * @return      A read-only pointer to the minimum value in the tree or NULL
 *              if tree is empty.
 */
const void*
cgs_rbt_min(const struct cgs_rbt* tree);

/**
 * cgs_rbt_max
 *
 * Get the maximum value in the tree.
 *
 * @param tree  Tree to find maximum value of.
 *
 * @return      A read-only pointer to the maximum value in the tree or NULL
 *              if tree is empty.
 */
const void*
cgs_rbt_max(const struct cgs_rbt* tree);

