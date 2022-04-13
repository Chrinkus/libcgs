#include "cmocka_headers.h"

#include "cgs_rbt.h"
#include "cgs_compare.h"	// cgs_int_cmp
#include "cgs_defs.h"		// CGS_ARRAY_LENGTH

static void rbt_new_test(void** state)
{
	(void)state;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);

	assert_non_null(tree);
	assert_int_equal(cgs_rbt_size(tree), 0);

	cgs_rbt_free(tree);
}

static void rbt_insert_test(void** state)
{
	(void)state;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);
	struct cgs_variant v = { 0 };

	// establish initial size
	assert_int_equal(cgs_rbt_size(tree), 0);

	// show increases through insertions
	cgs_variant_set_int(&v, 16);
	cgs_rbt_insert(tree, &v);
	assert_int_equal(cgs_rbt_size(tree), 1);

	cgs_variant_set_int(&v, 23);
	cgs_rbt_insert(tree, &v);
	assert_int_equal(cgs_rbt_size(tree), 2);

	cgs_rbt_free(tree);
}

static void rbt_minmax_test(void** state)
{
	(void)state;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);

	// insert single value
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 77);
	cgs_rbt_insert(tree, &v);

	// the single value should represent the min and max of the tree
	const int* min = cgs_rbt_min(tree);
	const int* max = cgs_rbt_max(tree);
	assert_int_equal(*min, 77);
	assert_int_equal(*max, 77);

	// insert a bunch of values
	int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
	for (size_t i = 0; i < CGS_ARRAY_LENGTH(arr); ++i) {
		cgs_variant_set_int(&v, arr[i]);
		cgs_rbt_insert(tree, &v);
	}

	// show that the min and max have changed correctly
	min = cgs_rbt_min(tree);
	max = cgs_rbt_max(tree);
	assert_int_equal(*min, -12);
	assert_int_equal(*max, 100);

	cgs_rbt_free(tree);
}

static void rbt_search_test(void** state)
{
	(void)state;

	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);
	struct cgs_variant v1 = { 0 };

	// insert a bunch of values
	int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
	for (size_t i = 0; i < CGS_ARRAY_LENGTH(arr); ++i) {
		cgs_variant_set_int(&v1, arr[i]);
		cgs_rbt_insert(tree, &v1);
	}
	
	// search for a value that is in the tree
	cgs_variant_set_int(&v1, 2);
	const struct cgs_variant* v2 = cgs_rbt_search(tree, &v1);
	assert_non_null(v2);
	assert_int_equal(*(const int*)cgs_variant_get(v2), 2);

	// search for a value that is not in the tree
	cgs_variant_set_int(&v1, 42);
	v2 = cgs_rbt_search(tree, &v1);
	assert_null(v2);

	cgs_variant_set_int(&v1, -12);
	v2 = cgs_rbt_search(tree, &v1);
	assert_non_null(v2);
	assert_int_equal(*(const int*)cgs_variant_get(v2), -12);

	cgs_variant_set_int(&v1, 100);
	v2 = cgs_rbt_search(tree, &v1);
	assert_non_null(v2);
	assert_int_equal(*(const int*)cgs_variant_get(v2), 100);

	cgs_rbt_free(tree);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(rbt_new_test),
		cmocka_unit_test(rbt_insert_test),
		cmocka_unit_test(rbt_minmax_test),
		cmocka_unit_test(rbt_search_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

