#include "cmocka_headers.h"

#include "cgs_bst.h"
#include "cgs_compare.h"

const int garr[] = { 10, 14, 2, -13, 37, -6, 100, 0, -1, 8, -67, 72 };
const int glen = sizeof(garr) / sizeof(garr[0]);

static void bst_int_new_test(void** state)
{
	(void)state;

        struct cgs_bst tree = { 0 };
        cgs_bst_new(&tree, cgs_int_cmp);

	assert_int_equal(cgs_bst_length(&tree), 0);

	cgs_bst_free(&tree);
}

static void bst_int_insert_test(void** state)
{
	(void)state;

        struct cgs_bst tree = { 0 };
        cgs_bst_new(&tree, cgs_int_cmp);

	struct cgs_variant var = { 0 };
	cgs_variant_set_int(&var, 5);
	assert_non_null(cgs_bst_insert(&tree, &var));
	assert_int_equal(cgs_bst_length(&tree), 1);

        cgs_variant_set_int(&var, 10);
        assert_non_null(cgs_bst_insert(&tree, &var));
        assert_int_equal(cgs_bst_length(&tree), 2);

	cgs_variant_set_int(&var, 5);
	assert_non_null(cgs_bst_insert(&tree, &var));
	assert_int_equal(cgs_bst_length(&tree), 2);     // no duplicate inserts

	cgs_bst_free(&tree);
}

void bst_int_search_test(void** state)
{
	(void)state;

        struct cgs_bst tree = { 0 };
        cgs_bst_new(&tree, cgs_int_cmp);

	struct cgs_variant var = { 0 };
	for (int i = 0; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(&tree, &var);
	}

	// re-usable pointers
	const struct cgs_variant* pv;
	const int* pi;

	for (int i = 0; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		pv = cgs_bst_search(&tree, &var);
		assert_non_null(pv);
		pi = cgs_variant_get(pv);
		assert_int_equal(*pi, garr[i]);
	}

	// find fail
	cgs_variant_set_int(&var, 53);
	pv = cgs_bst_search(&tree, &var);
	assert_null(pv);
	// insert it and find it
	cgs_bst_insert(&tree, &var);
	pv = cgs_bst_search(&tree, &var);
	assert_non_null(pv);
	pi = cgs_variant_get(pv);
	assert_int_equal(*pi, 53);

	// and again with negative value
	cgs_variant_set_int(&var, -42);
	pv = cgs_bst_search(&tree, &var);
	assert_null(pv);
	cgs_bst_insert(&tree, &var);
	pv = cgs_bst_search(&tree, &var);
	assert_non_null(pv);
	pi = cgs_variant_get(pv);
	assert_int_equal(*pi, -42);

	cgs_bst_free(&tree);
}

static void bst_int_minmax_test(void** state)
{
	(void)state;

        struct cgs_bst tree = { 0 };
        cgs_bst_new(&tree, cgs_int_cmp);

	struct cgs_variant var = { 0 };

	// Add elements in groups and check min and max values
	int i = 0;
	for ( ; i < 3; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(&tree, &var);
	}

	const int* min = cgs_bst_min(&tree);
	const int* max = cgs_bst_max(&tree);
	assert_int_equal(*min, 2);
	assert_int_equal(*max, 14);
	assert_int_equal(cgs_bst_length(&tree), 3);

	for ( ; i < 10; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(&tree, &var);
	}

	min = cgs_bst_min(&tree);
	max = cgs_bst_max(&tree);
	assert_int_equal(*min, -13);
	assert_int_equal(*max, 100);
	assert_int_equal(cgs_bst_length(&tree), 10);

	for ( ; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(&tree, &var);
	}

	min = cgs_bst_min(&tree);
	max = cgs_bst_max(&tree);
	assert_int_equal(*min, -67);
	assert_int_equal(*max, 100);
	assert_int_equal(cgs_bst_length(&tree), 12);

	cgs_bst_free(&tree);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(bst_int_new_test),
		cmocka_unit_test(bst_int_insert_test),
		cmocka_unit_test(bst_int_search_test),
		cmocka_unit_test(bst_int_minmax_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

