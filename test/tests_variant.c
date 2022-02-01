#include "cgs_variant.h"

#include "test_utils.h"

int variant_numeric_test(void* data)
{
	(void)data;

	struct cgs_variant v1 = { 0 };
	cgs_variant_set_int(&v1, 53);
	assert(v1.data.i == 53);

	const int* pi = cgs_variant_get(&v1);
	assert(*pi == 53);

	cgs_variant_set_double(&v1, 0.767);
	assert(v1.data.d > 0.7);
	assert(v1.data.d < 0.8);

	const double* pd = cgs_variant_get(&v1);
	assert(*pd > 0.7);
	assert(*pd < 0.8);

	return TEST_SUCCESS;
}

int variant_string_test(void* data)
{
	const char* p = data;
	size_t len = strlen(p);

	struct cgs_variant v1 = { 0 };

	cgs_variant_set_string(&v1, p);
	assert(strcmp(v1.data.s, p) == 0);
	assert(v1.data.s != p);
	assert(strlen(v1.data.s) == len);

	const char* s1 = cgs_variant_get(&v1);
	assert(strcmp(s1, p) == 0);
	assert(s1 != p);
	assert(strlen(s1) == len);

	cgs_variant_free_data(&v1);

	return TEST_SUCCESS;
}

struct point {
	int x, y, z;
};

int variant_data_test(void* data)
{
	(void)data;

	struct point* p1 = malloc(sizeof(struct point));
	p1->x = 33;
	p1->y = -7;
	p1->z = 28;

	struct cgs_variant v1 = { 0 };
	cgs_variant_set_data(&v1, p1);

	const struct point* p2 = cgs_variant_get(&v1);
	assert(p1 == p2);
	assert(p2->y == -7);

	p1->y = 55;
	assert(p2->y == 55);

	cgs_variant_free_data(&v1);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "variant_numeric", variant_numeric_test, NULL },
		{ "variant_string", variant_string_test, "Howdy" },
		{ "variant_data", variant_data_test, NULL },
	};

	return run_tests(tests, ARR_SIZE(tests));
}

