#pragma once

#include <stddef.h>

/**
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
 */

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Struct forward declarations
struct cgs_rbt;
struct cgs_rbt_node;
struct cgs_variant;

/**
 * cgs_rbt_cmp
 *
 * A three-way comparison function. Shares the same signature as qsort compare.
 */
typedef int (*cgs_rbt_cmp)(const void*, const void*);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Red-Black Tree Operations

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
 * @param tree	The tree to get the size of. Read only.
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

