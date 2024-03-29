/* cgs_bst.c
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
#include <stdlib.h>
#include <string.h>

#include "cgs_bst.h"

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

static struct cgs_bst_node*
cgs_bst_node_new(const struct cgs_variant* data)
{
        struct cgs_bst_node* node = malloc(sizeof(struct cgs_bst_node));
        if (!node)
                return NULL;

        memcpy(&node->data, data, sizeof(struct cgs_variant));
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        return node;
}

static void
cgs_bst_node_free(struct cgs_bst_node* node, CgsFreeFunc ff)
{
        if (!node)
                return;

        cgs_bst_node_free(node->left, ff);
        cgs_bst_node_free(node->right, ff);
        cgs_variant_free(&node->data, ff);
        free(node);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Tree Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_bst
cgs_bst_new(CgsCmp3Way cmp, CgsFreeFunc ff)
{
        return (struct cgs_bst){
                .root = NULL,
                .length = 0,
                .cmp = cmp,
                .ff = ff,
        };
}

void
cgs_bst_free(struct cgs_bst* tree)
{
        return cgs_bst_node_free(tree->root, tree->ff);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Tree Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

size_t
cgs_bst_length(const struct cgs_bst* tree);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * BST Tree Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_bst_insert(struct cgs_bst* tree, struct cgs_variant* data)
{
        struct cgs_bst_node* parent = NULL;
        int cmp = 0;
        for (struct cgs_bst_node* temp = tree->root; temp; parent = temp,
                        temp = cmp < 0 ? temp->left : temp->right) {
                cmp = tree->cmp(cgs_variant_get(data),
                                cgs_variant_get(&temp->data));
                if (cmp == 0)
                        return tree;
        }

        struct cgs_bst_node* node = cgs_bst_node_new(data);
        node->parent = parent;
        if (!parent)
                tree->root = node;
        else if (cmp < 0)
                parent->left = node;
        else
                parent->right = node;

        ++tree->length;

        return node;
}

const struct cgs_variant*
cgs_bst_search(const struct cgs_bst* tree, const struct cgs_variant* data)
{
        const struct cgs_bst_node* node = tree->root;

        for (int rc; node; node = rc < 0 ? node->left : node->right) {
                const void* a = cgs_variant_get(data);
                const void* b = cgs_variant_get(&node->data);
                rc = tree->cmp(a, b);
                if (rc == 0)
                        return &node->data;
        }
        return NULL;
}

const void*
cgs_bst_min(const struct cgs_bst* tree)
{
        const struct cgs_bst_node* node = tree->root;

        while (node && node->left)
                node = node->left;
        return node ? cgs_variant_get(&node->data) : NULL;
}

const void*
cgs_bst_max(const struct cgs_bst* tree)
{
        const struct cgs_bst_node* node = tree->root;

        while (node && node->right)
                node = node->right;
        return node ? cgs_variant_get(&node->data) : NULL;
}
