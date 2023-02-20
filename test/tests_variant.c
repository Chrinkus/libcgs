#include "cmocka_headers.h"

#include <stdlib.h>
#include <string.h>

#include "cgs_variant.h"
#include "cgs_string.h"         // complex data test

static void
numeric_test(void** state)
{
	(void)state;

	struct cgs_variant v1 = { 0 };
	cgs_variant_set_int(&v1, 53);
	assert_int_equal(v1.data.i, 53);

	const int* pi = cgs_variant_get(&v1);
	assert_int_equal(*pi, 53);

	cgs_variant_set_double(&v1, 0.767);
	assert_float_equal(v1.data.d, 0.767, 0.1);

	const double* pd = cgs_variant_get(&v1);
	assert_float_equal(*pd, 0.767, 0.1);
}

static void
string_test(void** state)
{
        (void)state;

	const char* p = "Howdy";
	const size_t len = strlen(p);

	struct cgs_variant v1 = { 0 };

	cgs_variant_set_cstr(&v1, p);
	assert_string_equal(v1.data.s, p);
	assert_ptr_not_equal(v1.data.s, p);
	assert_int_equal(strlen(v1.data.s), len);

	const char* s1 = cgs_variant_get(&v1);
	assert_string_equal(s1, p);
	assert_ptr_not_equal(s1, p);
	assert_int_equal(strlen(s1), len);

	cgs_variant_free(&v1, NULL);
}

struct point {
	int x, y, z;
};

static void
trivial_data_test(void** state)
{
	(void)state;

	struct point* p1 = malloc(sizeof(struct point));
	p1->x = 33;
	p1->y = -7;
	p1->z = 28;

	struct cgs_variant v1 = { 0 };
	cgs_variant_set_data(&v1, p1);

	const struct point* p2 = cgs_variant_get(&v1);
	assert_ptr_equal(p1, p2);
	assert_int_equal(p2->y, -7);

	p1->y = 55;
	assert_int_equal(p2->y, 55);

	cgs_variant_free(&v1, NULL);    // just 'free'ing data is enough
}

static void
complex_data_test(void** state)
{
        (void)state;
        struct cgs_string* p1 = malloc(sizeof(struct cgs_string));
        *p1 = cgs_string_new();
        cgs_string_from("Goldeneye", p1);

        struct cgs_variant v1 = { 0 };
        cgs_variant_set_data(&v1, p1);

        const struct cgs_string* p2 = cgs_variant_get(&v1);
        assert_string_equal("Goldeneye", cgs_string_data(p2));
        assert_ptr_equal(p1, p2);

        cgs_variant_free(&v1, cgs_string_free); // need custom free function
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(numeric_test),
                cmocka_unit_test(string_test),
                cmocka_unit_test(trivial_data_test),
                cmocka_unit_test(complex_data_test),
        };

	return cmocka_run_group_tests(tests, NULL, NULL);
}
