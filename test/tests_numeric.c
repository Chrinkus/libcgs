#include "cmocka_headers.h"

#include "cgs_numeric.h"

static void
numeric_is_prime_test(void** state)
{
        (void)state;

        assert_true(cgs_is_prime(2));
        assert_true(cgs_is_prime(3));
        assert_true(cgs_is_prime(17));
        assert_true(cgs_is_prime(101));
        assert_true(cgs_is_prime(7901));

        assert_false(cgs_is_prime(4));
        assert_false(cgs_is_prime(0));
        assert_false(cgs_is_prime(25));
        assert_false(cgs_is_prime(305));
        assert_false(cgs_is_prime(7153));
}

static void
numeric_next_prime_test(void** state)
{
        (void)state;

        assert_int_equal(cgs_next_prime(2), 3);
        assert_int_equal(cgs_next_prime(7), 11);
        assert_int_equal(cgs_next_prime(19), 23);
        assert_int_equal(cgs_next_prime(829), 839);
        assert_int_equal(cgs_next_prime(3500), 3511);
        assert_int_equal(cgs_next_prime(7883), 7901);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(numeric_is_prime_test),
		cmocka_unit_test(numeric_next_prime_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

