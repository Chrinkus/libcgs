#pragma once

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

