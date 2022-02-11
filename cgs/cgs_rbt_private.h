/* cgs_rbt_private.h
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

/**
 * enum cgs_rbt_color
 *
 * Constant values representing the possible colors of tree nodes.
 */
enum cgs_rbt_color { CGS_RBT_RED, CGS_RBT_BLACK };

/**
 * struct cgs_rbt_node
 *
 * A data structure representing a node in a red-black tree.
 */
struct cgs_rbt_node {
	struct cgs_variant data;
	struct cgs_rbt_node* parent;
	struct cgs_rbt_node* left;
	struct cgs_rbt_node* right;
	enum cgs_rbt_color color;
};

/**
 * struct cgs_rbt
 *
 * A data structure containing the elements of a red-black tree.
 */
struct cgs_rbt {
	struct cgs_rbt_node* root;
	size_t size;
	cgs_rbt_cmp cmp;
};

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
 */
struct cgs_rbt_node*
cgs_rbt_node_new(const struct cgs_variant* data);

/**
 * cgs_rbt_free
 *
 * Frees the memory of a node. Will first free left, then right children and
 * then free the data. Set's parent's pointer to node to NULL.
 *
 * @param node	The node to free.
 */
void
cgs_rbt_node_free(struct cgs_rbt_node* node);

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

