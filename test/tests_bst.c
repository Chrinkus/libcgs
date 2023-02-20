#include "cmocka_headers.h"

#include <stdlib.h>

#include "cgs_bst.h"
#include "cgs_compare.h"
#include "cgs_string.h"

const int ints[] = { 10, 14, 2, -13, 37, -6, 100, 0, -1, 8, -67, 72 };
const int ints_len = sizeof(ints) / sizeof(ints[0]);

const char* shows[] = {
        "Community",
        "Twin Peaks",
        "IT Crowd",
        "Dark",
        "It's Always Sunny in Philadelphia",
        "Star Trek",
        "The Mandalorian",
        "Modern Family",
        "Crash Landing on You",
};
const int shows_len = sizeof(shows) / sizeof(shows[0]);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void
bst_int_new_test(void** state)
{
	(void)state;

        struct cgs_bst tree = cgs_bst_new(cgs_int_cmp, NULL);

	assert_int_equal(cgs_bst_length(&tree), 0);

	cgs_bst_free(&tree);
}

static void
bst_int_insert_test(void** state)
{
	(void)state;

        struct cgs_bst tree = cgs_bst_new(cgs_int_cmp, NULL);

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

static void
bst_int_search_test(void** state)
{
	(void)state;

        struct cgs_bst tree = cgs_bst_new(cgs_int_cmp, NULL);

	struct cgs_variant var = { 0 };
	for (int i = 0; i < ints_len; ++i) {
		cgs_variant_set_int(&var, ints[i]);
		cgs_bst_insert(&tree, &var);
	}

	// re-usable pointers
	const struct cgs_variant* pv;
	const int* pi;

	for (int i = 0; i < ints_len; ++i) {
		cgs_variant_set_int(&var, ints[i]);
		pv = cgs_bst_search(&tree, &var);
		assert_non_null(pv);
		pi = cgs_variant_get(pv);
		assert_int_equal(*pi, ints[i]);
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

static void
bst_int_minmax_test(void** state)
{
	(void)state;

        struct cgs_bst tree = cgs_bst_new(cgs_int_cmp, NULL);

	struct cgs_variant var = { 0 };

	// Add elements in groups and check min and max values
	int i = 0;
	for ( ; i < 3; ++i) {
		cgs_variant_set_int(&var, ints[i]);
		cgs_bst_insert(&tree, &var);
	}

	const int* min = cgs_bst_min(&tree);
	const int* max = cgs_bst_max(&tree);
	assert_int_equal(*min, 2);
	assert_int_equal(*max, 14);
	assert_int_equal(cgs_bst_length(&tree), 3);

	for ( ; i < 10; ++i) {
		cgs_variant_set_int(&var, ints[i]);
		cgs_bst_insert(&tree, &var);
	}

	min = cgs_bst_min(&tree);
	max = cgs_bst_max(&tree);
	assert_int_equal(*min, -13);
	assert_int_equal(*max, 100);
	assert_int_equal(cgs_bst_length(&tree), 10);

	for ( ; i < ints_len; ++i) {
		cgs_variant_set_int(&var, ints[i]);
		cgs_bst_insert(&tree, &var);
	}

	min = cgs_bst_min(&tree);
	max = cgs_bst_max(&tree);
	assert_int_equal(*min, -67);
	assert_int_equal(*max, 100);
	assert_int_equal(cgs_bst_length(&tree), 12);

	cgs_bst_free(&tree);
}

static void
complex_data_test(void** state)
{
        (void)state;

        struct cgs_bst b1 = cgs_bst_new(cgs_string_cmp, cgs_string_free);

        for (int i = 0; i < shows_len; ++i) {
                struct cgs_string* p = malloc(sizeof(struct cgs_string));
                *p = cgs_string_new();
                cgs_string_from(shows[i], p);

                struct cgs_variant v1 = { 0 };
                cgs_variant_set_data(&v1, p);
                assert_non_null(cgs_bst_insert(&b1, &v1));
        }

        assert_int_equal(cgs_bst_length(&b1), 9);

        const struct cgs_string* min = cgs_bst_min(&b1);
        assert_string_equal(cgs_string_data(min), "Community");

        const struct cgs_string* max = cgs_bst_max(&b1);
        assert_string_equal(cgs_string_data(max), "Twin Peaks");

        struct cgs_string* p2 = malloc(sizeof(struct cgs_string));
        *p2 = cgs_string_new();
        cgs_string_from("Two and a Half Men", p2);
        struct cgs_variant v2 = { 0 };
        cgs_variant_set_data(&v2, p2);

        assert_null(cgs_bst_search(&b1, &v2));
        cgs_variant_free(&v2, cgs_string_free);

        cgs_bst_free(&b1);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(bst_int_new_test),
		cmocka_unit_test(bst_int_insert_test),
		cmocka_unit_test(bst_int_search_test),
		cmocka_unit_test(bst_int_minmax_test),
		cmocka_unit_test(complex_data_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
