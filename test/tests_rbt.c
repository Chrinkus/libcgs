#include "cgs_rbt.h"
#include "cgs_test.h"

int int_cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int rbt_new_test(void* data)
{
	(void)data;

	struct cgs_rbt* tree = cgs_rbt_new(int_cmp);

	assert(tree != NULL);
	assert(cgs_rbt_size(tree) == 0);

	cgs_rbt_free(tree);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "rbt_new", rbt_new_test, NULL },
	};

	return cgs_run_tests(tests);
}

