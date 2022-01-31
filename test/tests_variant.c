#include "cgs_variant.h"

#include "test_utils.h"

struct point {
	int x;
	int y;
};

int variant_set_data_test(void* data)
{
	(void)data;

	struct cgs_variant v1 = { .type = CGS_TYPE_INT };
	int n = 53;
	cgs_variant_set_data(&v1, &n);
	assert(v1.data.i == 53);

	struct cgs_variant v2 = { .type = CGS_TYPE_DOUBLE };
	double x = 0.767;
	cgs_variant_set_data(&v2, &x);
	assert(v2.data.d > 0.7);
	assert(v2.data.d < 0.8);

	struct cgs_variant v3 = { .type = CGS_TYPE_STRING };
	char* s = "Howdy";
	cgs_variant_set_data(&v3, s);
	assert(strcmp(v3.data.s, "Howdy") == 0);

	struct cgs_variant v4 = { .type = CGS_TYPE_DATA };
	struct point p1 = { .x = 5, .y = -3 };
	cgs_variant_set_data(&v4, &p1);
	assert(((struct point*)v4.data.v)->x == 5);
	assert(((struct point*)v4.data.v)->y == -3);

	return TEST_SUCCESS;
}

int variant_set_type_test(void* data)
{
	(void)data;

	int n = 42;
	double x = 3.14159;

	struct cgs_variant v = { 0 };

	cgs_variant_set_type(&v, CGS_TYPE_INT);
	cgs_variant_set_data(&v, &n);
	int* pi = (int*)cgs_variant_get(&v);
	assert(*pi == n);

	cgs_variant_set_type(&v, CGS_TYPE_DOUBLE);
	cgs_variant_set_data(&v, &x);
	double* pd = (double*)cgs_variant_get(&v);
	assert(*pd > 3.0);
	assert(*pd < 3.2);
	
	return TEST_SUCCESS;
}

int variant_get_test(void* data)
{
	(void)data;

	int n = -100;
	double x = 43.787;
	char* s = "Tuesday";
	struct point* pt = malloc(sizeof(struct point));
	pt->x = 808;
	pt->y = -989;

	struct cgs_variant v1 = { .type = CGS_TYPE_INT };
	struct cgs_variant v2 = { .type = CGS_TYPE_DOUBLE };
	struct cgs_variant v3 = { .type = CGS_TYPE_STRING };
	struct cgs_variant v4 = { .type = CGS_TYPE_DATA };

	cgs_variant_set_data(&v1, &n);
	cgs_variant_set_data(&v2, &x);
	cgs_variant_set_data(&v3, s);
	cgs_variant_set_data(&v4, pt);

	int* pi = (int*)cgs_variant_get(&v1);
	double* pd = (double*)cgs_variant_get(&v2);
	char* ps = (char*)cgs_variant_get(&v3);
	struct point* ppt = (struct point*)cgs_variant_get(&v4);

	assert(*pi == n);
	assert(*pd == x);
	assert(strcmp(ps, s) == 0);
	assert(ppt->x == 808 && ppt->y == -989);

	free(pt);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "variant_set_data", variant_set_data_test, NULL },
		{ "variant_set_type", variant_set_type_test, NULL },
		{ "variant_get", variant_get_test, NULL },
	};

	return run_tests(tests, ARR_SIZE(tests));
}

