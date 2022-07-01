#include "cmocka_headers.h"

#include <stdlib.h>     /* EXIT_FAILURE */

#include "cgs_error.h"

static void
error_retfail_test(void** state)
{
        (void)state;

        int ret = cgs_error_retfail("Retfail test");
        assert_int_equal(ret, EXIT_FAILURE);
}

static void
error_retnull_test(void** state)
{
        (void)state;

        void* ret = cgs_error_retnull("Retnull test");
        assert_null(ret);
}

static void
error_retbool_test(void** state)
{
        (void)state;

        int ret = cgs_error_retbool("Retbool test");
        assert_int_equal(ret, 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(error_retfail_test),
		cmocka_unit_test(error_retnull_test),
		cmocka_unit_test(error_retbool_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

