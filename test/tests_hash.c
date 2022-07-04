#include "cmocka_headers.h"

#include "cgs_hash.h"

static void
hash_new_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h, cgs_hash_func_string));

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

