#include "cgs_bst.h"

#include <stdlib.h>
#include <assert.h>

enum { TEST_SUCCESS, TEST_FAILURE };

int bst_new_test(void)
{
	struct cgs_bst* tree = cgs_bst_new();
	assert(tree != NULL);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int main(void)
{
	assert(bst_new_test() == 0);

	return TEST_SUCCESS;
}

