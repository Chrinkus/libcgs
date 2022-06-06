/* cgs.h
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
 * cgs_rbt_private.h
 *
 * This file contains the private definitions of the enums and structs used to
 * implement a red-black tree. There are also additional function declarations
 * to be used for testing the data structure.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#pragma once

#include "cgs_rbt.h"
#include "cgs_variant.h"
#include "cgs_defs.h"

/**
 * cgs_rbt_root
 *
 * Get a read-only pointer to the tree's root node. For testing purposes only.
 *
 * @param tree	The tree to get the root of. Read-only.
 *
 * @return	A pointer to the node at the root.
 */
const struct cgs_rbt_node*
cgs_rbt_root(const struct cgs_rbt* tree);

/**
 * cgs_rbt_node_new
 *
 * Create a new rb-tree node with the given data. Sets color to CGS_RBT_RED
 * and all pointers to NULL.
 *
 * @param data	A variant with data of the appropriate type.
 *
 * @return	A pointer to a newly allocated node.
struct cgs_rbt_node*
cgs_rbt_node_new(const struct cgs_variant* data);
 */

/**
 * cgs_rbt_free
 *
 * Frees the memory of a node. Will first free left, then right children and
 * then free the data. Set's parent's pointer to node to NULL.
 *
 * @param node	The node to free.
void
cgs_rbt_node_free(struct cgs_rbt_node* node);
 */

/**
 * cgs_rbt_node_is_red
 *
 * A test function to determine if a given node is red.
 *
 * @param node	A read-only pointer to the node to test.
 *
 * @return	An integer boolean indicating true (1) or false (0).
 */
int
cgs_rbt_node_is_red(const struct cgs_rbt_node* node);

/**
 * cgs_rbt_node_is_black
 *
 * A test function to determine if a given node is black.
 *
 * @param node	A read-only pointer to the node to test.
 *
 * @return	An integer boolean indicating true (1) or false (0).
 */
int
cgs_rbt_node_is_black(const struct cgs_rbt_node* node);

/**
 * cgs_rbt_node_black_height
 *
 * A recursive test function to count the number of black nodes on a simple
 * path from the given node. Counts the left paths and right paths first to
 * compare. If they are equal and valid counts then return the count plus
 * 1 if NODE is black.
 *
 * @param node	The node to test.
 *
 * @return	The number of black nodes beneath NODE + 1 if NODE itself is
 * 		black or -1 on unequal heights.
 */
int
cgs_rbt_node_black_height(const struct cgs_rbt_node* node);

/**
 * cgs_rbt_node_red_children_test
 *
 * A test function to check whether a red node has red children.
 *
 * @param node	The node to test.
 *
 * @return	An integer boolean.
 */
int
cgs_rbt_node_red_children_test(const struct cgs_rbt_node* node);

