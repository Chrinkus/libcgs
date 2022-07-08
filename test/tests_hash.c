#include "cmocka_headers.h"

#include "cgs_hash.h"

static void
hash_new_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        assert_int_equal(cgs_hash_length(&h), 0);

        cgs_hash_free(&h);
}

static void
hash_lookup_fail_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        assert_null(cgs_hash_lookup(&h, "article"));
        assert_null(cgs_hash_lookup(&h, "section"));
        assert_null(cgs_hash_lookup(&h, "h1"));
        assert_null(cgs_hash_lookup(&h, "div"));

        cgs_hash_free(&h);
}

static void
hash_get_success_test(void** state)
{
        (void)state;
        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        struct cgs_variant* pv = NULL;
        pv = cgs_hash_get(&h, "Wayne Gretzky");
        assert_non_null(pv);
        cgs_variant_set_int(pv, 894);

        pv = cgs_hash_get(&h, "Gordie Howe");
        assert_non_null(pv);
        cgs_variant_set_int(pv, 801);

        pv = cgs_hash_get(&h, "Alex Ovechkin");
        assert_non_null(pv);
        cgs_variant_set_int(pv, 780);

        assert_int_equal(cgs_hash_length(&h), 3);

        cgs_hash_free(&h);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(hash_new_test),
                cmocka_unit_test(hash_lookup_fail_test),
		cmocka_unit_test(hash_get_success_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

