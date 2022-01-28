#pragma once

#include <stddef.h>

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

/*
 * Struct forward declarations
 */
struct cgs_bst;
struct cgs_bst_node;

/**
 * cgs_bst_new
 *
 * Allocate a new binary search tree and return it. Size will be 0 and the root
 * will be NULL.
 *
 * @return	An empty binary search tree.
 */
struct cgs_bst* cgs_bst_new(void);

/**
 * cgs_bst_free
 *
 * Deallocate a binary search tree. Will first free the elements of the tree
 * then the tree itself.
 *
 * @param tree	Tree to be freed. May be NULL.
 */
void cgs_bst_free(struct cgs_bst* tree);

/**
 * cgs_bst_size
 *
 * Get the size of the tree.
 *
 * @param tree	The tree to get the size of. Read only.
 *
 * @return	The size of the tree.
 */
size_t cgs_bst_size(const struct cgs_bst* tree);

/**
 * cgs_bst_insert
 *
 * Insert a node into the tree.
 *
 * @param tree	Tree to insert node into.
 * @param val	Value to insert into tree.
 *
 * @return	Pointer to inserted node if successful, NULL on failure.
 */
struct cgs_bst_node* cgs_bst_insert(struct cgs_bst* tree, int val);

/**
 * cgs_bst_min
 *
 * Get the minimum value in the tree.
 *
 * @param tree	Tree to find minimum value of.
 *
 * @return	Minimum value in the tree.
 */
int cgs_bst_min(const struct cgs_bst* tree);

/**
 * cgs_bst_max
 *
 * Get the maximum value in the tree.
 *
 * @param tree	Tree to find maximum value of.
 *
 * @return	Maximum value in the tree.
 */
int cgs_bst_max(const struct cgs_bst* tree);

