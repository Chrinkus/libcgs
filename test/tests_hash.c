#include "cmocka_headers.h"

#include "cgs_hash.h"
#include "cgs_compare.h"        /* cgs_str_cmp */

static void
hash_new_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h, cgs_string_hash, cgs_str_cmp));

        cgs_hash_free(&h);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(hash_new_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

