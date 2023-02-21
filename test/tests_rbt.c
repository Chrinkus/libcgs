#include "cmocka_headers.h"

#include "cgs_rbt.h"
#include "cgs_compare.h"	// cgs_int_cmp
#include "cgs_defs.h"		// CGS_ARRAY_LENGTH

#include <stdlib.h>
#include "cgs_string.h"

static void
rbt_new_test(void** state)
{
        (void)state;

        struct cgs_rbt tree = cgs_rbt_new(cgs_int_cmp, NULL);

        assert_int_equal(cgs_rbt_length(&tree), 0);

        cgs_rbt_free(&tree);
}

static void
rbt_insert_test(void** state)
{
        (void)state;

        struct cgs_rbt tree = cgs_rbt_new(cgs_int_cmp, NULL);

	// establish initial size
        assert_int_equal(cgs_rbt_length(&tree), 0);

	// show increases through insertions
	struct cgs_variant var = { 0 };
	cgs_variant_set_int(&var, 16);

	assert_non_null(cgs_rbt_insert(&tree, &var));
	assert_int_equal(cgs_rbt_length(&tree), 1);

	cgs_variant_set_int(&var, 23);

	assert_non_null(cgs_rbt_insert(&tree, &var));
	assert_int_equal(cgs_rbt_length(&tree), 2);

	cgs_rbt_free(&tree);
}

static void
rbt_minmax_test(void** state)
{
        (void)state;

        struct cgs_rbt tree = cgs_rbt_new(cgs_int_cmp, NULL);

        // insert single value
        struct cgs_variant v = { 0 };
        cgs_variant_set_int(&v, 77);
        cgs_rbt_insert(&tree, &v);

        // the single value should represent the min and max of the tree
        const int* min = cgs_rbt_min(&tree);
        const int* max = cgs_rbt_max(&tree);
        assert_int_equal(*min, 77);
        assert_int_equal(*max, 77);

        // insert a bunch of values
        int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
        for (size_t i = 0; i < CGS_ARRAY_LENGTH(arr); ++i) {
                cgs_variant_set_int(&v, arr[i]);
                cgs_rbt_insert(&tree, &v);
        }

        // show that the min and max have changed correctly
        min = cgs_rbt_min(&tree);
        max = cgs_rbt_max(&tree);
        assert_int_equal(*min, -12);
        assert_int_equal(*max, 100);

        cgs_rbt_free(&tree);
}

static void
rbt_search_test(void** state)
{
        (void)state;

        struct cgs_rbt tree = cgs_rbt_new(cgs_int_cmp, NULL);
        struct cgs_variant v1 = { 0 };

        // insert a bunch of values
        int arr[] = { 2, 7, -3, 19, -1, 0, -12, 6, 100, 15 };
        for (size_t i = 0; i < CGS_ARRAY_LENGTH(arr); ++i) {
                cgs_variant_set_int(&v1, arr[i]);
                cgs_rbt_insert(&tree, &v1);
        }

        // search for a value that is in the tree
        cgs_variant_set_int(&v1, 2);

        const struct cgs_variant* pv = NULL;
        const int* pi = NULL;

        pv = cgs_rbt_search(&tree, &v1);
        assert_non_null(pv);
        pi = cgs_variant_get(pv);
        assert_int_equal(*pi, 2);

        // search for a value that is not in the tree
        cgs_variant_set_int(&v1, 42);
        pv = cgs_rbt_search(&tree, &v1);
        assert_null(pv);

        cgs_variant_set_int(&v1, -12);
        pv = cgs_rbt_search(&tree, &v1);
        assert_non_null(pv);
        pi = cgs_variant_get(pv);
        assert_int_equal(*pi, -12);

        cgs_variant_set_int(&v1, 100);
        pv = cgs_rbt_search(&tree, &v1);
        assert_non_null(pv);
        pi = cgs_variant_get(pv);
        assert_int_equal(*pi, 100);

        cgs_rbt_free(&tree);
}

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

static void
complex_data_test(void** state)
{
        (void)state;

        struct cgs_rbt t1 = cgs_rbt_new(cgs_string_cmp, cgs_string_free);

        for (int i = 0; i < shows_len; ++i) {
                struct cgs_string* p1 = malloc(sizeof(struct cgs_string));
                *p1 = cgs_string_new();
                cgs_string_from(shows[i], p1);

                struct cgs_variant v1 = { 0 };
                cgs_variant_set_data(&v1, p1);
                assert_non_null(cgs_rbt_insert(&t1, &v1));
        }

        assert_int_equal(cgs_rbt_length(&t1), shows_len);

        const struct cgs_string* min = cgs_rbt_min(&t1);
        assert_string_equal(cgs_string_data(min), "Community");

        const struct cgs_string* max = cgs_rbt_max(&t1);
        assert_string_equal(cgs_string_data(max), "Twin Peaks");

        struct cgs_string* p2 = malloc(sizeof(struct cgs_string));
        *p2 = cgs_string_new();
        cgs_string_from("Two and a Half Men", p2);
        struct cgs_variant v2 = { 0 };
        cgs_variant_set_data(&v2, p2);

        assert_null(cgs_rbt_search(&t1, &v2));
        cgs_variant_free(&v2, cgs_string_free);

        cgs_rbt_free(&t1);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(rbt_new_test),
		cmocka_unit_test(rbt_insert_test),
		cmocka_unit_test(rbt_minmax_test),
		cmocka_unit_test(rbt_search_test),
		cmocka_unit_test(complex_data_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

