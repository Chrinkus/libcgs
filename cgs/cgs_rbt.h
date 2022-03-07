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
#include "cgs_compare.h"

/*
 * Struct forward declarations
 */
struct cgs_rbt;
struct cgs_rbt_node;

/**
 * cgs_rbt_cmp
 *
 * A three-way comparison function. Shares the same signature as qsort compare.
 */
typedef int (*cgs_rbt_cmp)(const void*, const void*);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Red-Black Tree Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_rbt_new
 *
 * Allocate a new red-black tree and return it. Size will be 0, the root
 * will be NULL and cmp will be set.
 *
 * @param cmp	The comparison function to order the tree with.
 *
 * @return	An empty red-black tree.
 */
struct cgs_rbt*
cgs_rbt_new(cgs_rbt_cmp cmp);

/**
 * cgs_rbt_free
 *
 * Deallocate a red-black tree. Will first free the elements of the tree
 * then the tree itself.
 *
 * @param tree	Tree to be freed. May be NULL.
 */
void
cgs_rbt_free(struct cgs_rbt* tree);

/**
 * cgs_rbt_size
 *
 * Get the size of the tree.
 *
 * @param tree	The tree to get the size of. Read-only.
 *
 * @return	The size of the tree.
 */
size_t
cgs_rbt_size(const struct cgs_rbt* tree);

/**
 * cgs_rbt_insert
 *
 * Insert a node into the tree.
 *
 * @param tree	Tree to insert node into.
 * @param val	Value to insert into tree.
 *
 * @return	Pointer to inserted node if successful, NULL on failure.
 */
struct cgs_rbt_node*
cgs_rbt_insert(struct cgs_rbt* tree, struct cgs_variant* data);

/**
 * cgs_rbt_min
 *
 * Get the minimum value in the tree.
 *
 * @param tree	Tree to find minimum value of.
 *
 * @return	Minimum value in the tree.
 */
const void*
cgs_rbt_min(const struct cgs_rbt* tree);

/**
 * cgs_rbt_max
 *
 * Get the maximum value in the tree.
 *
 * @param tree	Tree to find maximum value of.
 *
 * @return	Maximum value in the tree.
 */
const void*
cgs_rbt_max(const struct cgs_rbt* tree);

/**
 * cgs_rbt_search
 *
 * Search the tree for an entry. Uses the 3-way comparison function of the
 * tree to find a matching variant.
 *
 * @param tree	Tree to search.
 * @param data	Data to find in the tree.
 *
 * @return	A read-only pointer to the variant that matches the search
 * 		data or NULL if not found.
 */
const struct cgs_variant*
cgs_rbt_search(const struct cgs_rbt* tree, const struct cgs_variant* data);

