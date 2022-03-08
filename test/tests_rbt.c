#include "cgs_rbt.h"
#include "cgs_compare.h"
#include "cgs_test.h"

int rbt_new_test(void* data)
{
	(void)data;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);

	assert(tree != NULL);
	assert(cgs_rbt_size(tree) == 0);

	cgs_rbt_free(tree);

	return TEST_SUCCESS;
}

int rbt_insert_test(void* data)
{
	(void)data;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);
	assert(cgs_rbt_size(tree) == 0);

	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 16);

	cgs_rbt_insert(tree, &v);
	assert(cgs_rbt_size(tree) == 1);

	cgs_variant_set_int(&v, 23);
	cgs_rbt_insert(tree, &v);
	assert(cgs_rbt_size(tree) == 2);

	cgs_rbt_free(tree);

	return TEST_SUCCESS;
}

int rbt_minmax_test(void* data)
{
	(void)data;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);

	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 77);
	cgs_rbt_insert(tree, &v);

	const int* min = cgs_rbt_min(tree);
	const int* max = cgs_rbt_max(tree);
	assert(*min == 77);
	assert(*max == 77);

	int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
	for (int i = 0; i < 10; ++i) {
		cgs_variant_set_int(&v, arr[i]);
		cgs_rbt_insert(tree, &v);
	}

	min = cgs_rbt_min(tree);
	max = cgs_rbt_max(tree);
	assert(*min == -12);
	assert(*max == 100);

	cgs_rbt_free(tree);
	return TEST_SUCCESS;
}

int rbt_search_test(void* data)
{
	(void)data;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);
	struct cgs_variant v1 = { 0 };

	int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
	for (int i = 0; i < 10; ++i) {
		cgs_variant_set_int(&v1, arr[i]);
		cgs_rbt_insert(tree, &v1);
	}
	
	cgs_variant_set_int(&v1, 2);
	const struct cgs_variant* v2 = cgs_rbt_search(tree, &v1);
	assert(v2 != NULL);
	assert(*(const int*)cgs_variant_get(v2) == 2);

	cgs_variant_set_int(&v1, 42);
	v2 = cgs_rbt_search(tree, &v1);
	assert(v2 == NULL);

	cgs_variant_set_int(&v1, -12);
	v2 = cgs_rbt_search(tree, &v1);
	assert(v2 != NULL);
	assert(*(const int*)cgs_variant_get(v2) == -12);

	cgs_variant_set_int(&v1, 100);
	v2 = cgs_rbt_search(tree, &v1);
	assert(v2 != NULL);
	assert(*(const int*)cgs_variant_get(v2) == 100);

	cgs_rbt_free(tree);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "rbt_new", rbt_new_test, NULL },
		{ "rbt_insert", rbt_insert_test, NULL },
		{ "rbt_minmax", rbt_minmax_test, NULL },
		{ "rbt_search", rbt_search_test, NULL },
	};

	return cgs_run_tests(tests);
}

