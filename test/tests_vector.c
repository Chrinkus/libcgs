#include "cmocka_headers.h"

#include <stdlib.h>	// malloc, free

#include "cgs_vector.h"

#include "cgs_compare.h"
#include "cgs_string_utils.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Setups and teardowns
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum { NUM_RANDOMS = 10, NUM_IOTAS = 10, NUM_TOWNS = 5 };

static int setup_random(void** state)
{
	int* vi = malloc(sizeof(int) * NUM_RANDOMS);
	if (!vi)
		return -1;
	vi[0] = 37;
	vi[1] = 2;
	vi[2] = 10;
	vi[3] = -13;
	vi[4] = -22;
	vi[5] = 5;
	vi[6] = 18;
	vi[7] = -1;
	vi[8] = 0;
	vi[9] = 30;

	*state = vi;

	return 0;
}

static int setup_iota(void** state)
{
	int* vi = malloc(sizeof(int) * NUM_IOTAS);
	if (!vi)
		return -1;

	for (int i = 0, n = 1; i < NUM_IOTAS; ++i, ++n)
		vi[i] = n;

	*state = vi;

	return 0;
}

static int setup_towns(void** state)
{
	const char** vs = malloc(sizeof(char*) * NUM_TOWNS);
	if (!vs)
		return -1;

	vs[0] = "Cooks Creek";
	vs[1] = "Anola";
	vs[2] = "Hazelridge";
	vs[3] = "Oakbank";
	vs[4] = "Dugald";

	*state = vs;

	return 0;
}

static int teardown_ptr(void** state)
{
	free(*state);
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Tests
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void vector_new_test(void** state)
{
	(void)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
	assert_int_equal(vi.length, 0);
	assert_int_equal(vi.capacity, 0);
	assert_int_equal(vi.element_size, sizeof(int));
	assert_null(vi.data);

        struct cgs_vector vd = cgs_vector_new(sizeof(double));
	assert_int_equal(vd.length, 0);
	assert_int_equal(vd.capacity, 0);
	assert_int_equal(vd.element_size, sizeof(double));
	assert_null(vd.data);

        struct cgs_vector vs = cgs_vector_new(sizeof(char*));
	assert_int_equal(vs.length, 0);
	assert_int_equal(vs.capacity, 0);
	assert_int_equal(vs.element_size, sizeof(char*));
	assert_null(vs.data);
}

static void vector_push_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
	for (int i = 0; i < NUM_RANDOMS; ++i)
		assert_non_null(cgs_vector_push(&vi, &ints[i]));

	assert_int_equal(cgs_vector_length(&vi), NUM_RANDOMS);

	cgs_vector_free(&vi);
}

static void vector_remove_test(void** state)
{
        (void)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
        for (int i = 1; i <= 10; ++i)
                cgs_vector_push(&vi, &i);

        assert_int_equal(cgs_vector_length(&vi), 10);

        const int* pi = cgs_vector_get(&vi, 5);
        assert_int_equal(*pi, 6);

        cgs_vector_remove(&vi, 5);
        assert_int_equal(cgs_vector_length(&vi), 9);
        pi = cgs_vector_get(&vi, 5);
        assert_int_equal(*pi, 7);

        cgs_vector_remove(&vi, 0);
        assert_int_equal(cgs_vector_length(&vi), 8);
        pi = cgs_vector_get(&vi, 5);
        assert_int_equal(*pi, 8);
        pi = cgs_vector_get(&vi, 0);
        assert_int_equal(*pi, 2);

        cgs_vector_remove(&vi, 7);
        assert_int_equal(cgs_vector_length(&vi), 7);
        pi = cgs_vector_get(&vi, 5);
        assert_int_equal(*pi, 8);

        cgs_vector_free(&vi);
}

static void vector_remove_fast_test(void** state)
{
        (void)state;

        struct cgs_vector v = cgs_vector_new(sizeof(int));
        for (int i = 1; i <= 10; ++i)
                cgs_vector_push(&v, &i);

        assert_int_equal(cgs_vector_length(&v), 10);

        const int* pi = cgs_vector_get(&v, 4);
        assert_int_equal(*pi, 5);

        cgs_vector_remove_fast(&v, 4);
        assert_int_equal(cgs_vector_length(&v), 9);
        pi = cgs_vector_get(&v, 4);
        assert_int_equal(*pi, 10);

        cgs_vector_remove_fast(&v, 4);
        assert_int_equal(cgs_vector_length(&v), 8);
        pi = cgs_vector_get(&v, 4);
        assert_int_equal(*pi, 9);

        cgs_vector_remove_fast(&v, 7);
        assert_int_equal(cgs_vector_length(&v), 7);
        pi = cgs_vector_get(&v, 4);
        assert_int_equal(*pi, 9);

        cgs_vector_free(&v);
}

static void vector_clear_test(void** state)
{
        (void)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
        for (int i = 1; i <= 10; ++i)
                cgs_vector_push(&vi, &i);

        const int* data1 = cgs_vector_data(&vi);
        size_t cap1 = vi.capacity;

        assert_int_equal(cgs_vector_length(&vi), 10);
        cgs_vector_clear(&vi);
        assert_int_equal(cgs_vector_length(&vi), 0);

        const int* data2 = cgs_vector_data(&vi);
        size_t cap2 = vi.capacity;

        assert_ptr_equal(data1, data2);
        assert_int_equal(cap1, cap2);

        cgs_vector_free(&vi);
}

static void vector_sort_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = { 0 };
        cgs_vector_new_from_array(&vi, sizeof(ints[0]), ints, NUM_RANDOMS);

	cgs_vector_sort(&vi, cgs_int_cmp);

	int min = *(int*)cgs_vector_get(&vi, 0);
	int max = *(int*)cgs_vector_get(&vi, 9);
	assert_int_equal(min, -22);
	assert_int_equal(max, 37);

	cgs_vector_free(&vi);
}

static void vector_find_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = { 0 };
        cgs_vector_new_from_array(&vi, sizeof(ints[0]), ints, NUM_RANDOMS);

	int x = 99;
	int* found = cgs_vector_find(&vi, cgs_int_pred, &x);
	assert_null(found);
	x = -13;
	found = cgs_vector_find(&vi, cgs_int_pred, &x);
	assert_non_null(found);
	assert_ptr_not_equal(found, &x);
	assert_int_equal(*found, -13);

	cgs_vector_free(&vi);
}

static void vector_xfer_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = { 0 };
        cgs_vector_new_from_array(&vi, sizeof(ints[0]), ints, NUM_RANDOMS);

	size_t len = 0;
	int* out = cgs_vector_xfer(&vi, &len);

	assert_int_equal(len, 10);
	assert_non_null(out);
	assert_ptr_not_equal(out, ints);

	for (size_t i = 0; i < len; ++i)
		assert_int_equal(out[i], ints[i]);

	free(out);
}

static void vector_copy_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector v1 = cgs_vector_new(sizeof(int));
	for (int i = 0; i < NUM_RANDOMS; ++i)
		cgs_vector_push(&v1, &ints[i]);

        struct cgs_vector v2 = cgs_vector_new(0);
        assert_non_null(cgs_vector_copy(&v1, &v2));
        assert_int_not_equal(v2.length, 0);
        assert_int_not_equal(v2.capacity, 0);
        assert_int_not_equal(v2.element_size, 0);
        assert_non_null(v2.data);

        assert_int_equal(v2.length, v1.length);
        assert_int_equal(v2.capacity, v1.capacity);
        assert_int_equal(v2.element_size, v1.element_size);
        assert_ptr_not_equal(v2.data, v1.data);

        for (size_t i = 0; i < cgs_vector_length(&v1); ++i) {
                const int* p1 = cgs_vector_get(&v1, i);
                const int* p2 = cgs_vector_get(&v2, i);
                assert_int_equal(*p1, *p2);
        }

        cgs_vector_free(&v1);
        cgs_vector_free(&v2);
}

static void vector_new_from_array_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = { 0 };
        void* res = cgs_vector_new_from_array(&vi, sizeof(ints[0]), ints,
                        NUM_RANDOMS);
        assert_non_null(res);

	assert_int_equal(cgs_vector_length(&vi), NUM_RANDOMS);

	for (size_t i = 0; i < cgs_vector_length(&vi); ++i)
		assert_int_equal(*(int*)cgs_vector_get(&vi, i), ints[i]);

	cgs_vector_free(&vi);
}

static void vector_iter_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_vector vi = { 0 };
        cgs_vector_new_from_array(&vi, sizeof(ints[0]), ints, NUM_IOTAS);

	const int* b = cgs_vector_begin(&vi);
	assert_non_null(b);

	const int* e = cgs_vector_end(&vi);
	assert_non_null(e);

	int sum = 0;
	while (b != e)
		sum += *b++;

	assert_int_equal(sum, 55);

	cgs_vector_free(&vi);
}

static void vector_strings_test(void** state)
{
	const char** strs = *(const char***)state;

        struct cgs_vector vs = cgs_vector_new(sizeof(char*));
	for (int i = 0; i < NUM_TOWNS; ++i) {
		char* p = cgs_strdup(strs[i]);
		cgs_vector_push(&vs, &p);
	}

	assert_int_equal(cgs_vector_length(&vs), NUM_TOWNS);

	// Get :(
	const char*const* p = cgs_vector_get(&vs, 0);
	assert_string_equal(*p, strs[0]);

	// Iterators :)
	CgsStrIter b = cgs_vector_begin(&vs);
	CgsStrIter e = cgs_vector_end(&vs);
	for (CgsStrIter s = strs; b != e; ++b, ++s)
		assert_string_equal(*b, *s);

	cgs_vector_free_all(&vs);
}

void accum_int(const void* p, size_t i, void* data)
{
        int val = *(const int*)p;
        int* sum = data;
        (void)i;

        *sum += val;
}

static void vector_foreach_test(void** state)
{
        (void)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
        for (int i = 0; i < 10; ++i)
                cgs_vector_push(&vi, &i);

        int sum = 0;
        cgs_vector_foreach(&vi, accum_int, &sum);

        assert_int_equal(sum, 45);

        cgs_vector_free(&vi);
}

void double_int(void* ele, size_t i, void* data)
{
        int* p = ele;
        (void)i;
        (void)data;

        *p *= 2;
}

static void vector_transform_test(void** state)
{
        (void)state;

        struct cgs_vector vi = cgs_vector_new(sizeof(int));
        for (int i = 0; i < 10; ++i)
                cgs_vector_push(&vi, &i);

        cgs_vector_transform(&vi, double_int, NULL);

        const int* p = cgs_vector_get(&vi, 0);
        assert_int_equal(p[0], 0);
        assert_int_equal(p[1], 2);
        assert_int_equal(p[5], 10);
        assert_int_equal(p[9], 18);

        cgs_vector_free(&vi);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(vector_new_test),
		cmocka_unit_test(vector_push_test),
		cmocka_unit_test(vector_remove_test),
		cmocka_unit_test(vector_remove_fast_test),
                cmocka_unit_test(vector_clear_test),
		cmocka_unit_test(vector_sort_test),
		cmocka_unit_test(vector_find_test),
		cmocka_unit_test(vector_xfer_test),
		cmocka_unit_test(vector_copy_test),
		cmocka_unit_test(vector_new_from_array_test),
		cmocka_unit_test_setup_teardown(vector_iter_test, setup_iota,
				teardown_ptr),
		cmocka_unit_test_setup_teardown(vector_strings_test,
				setup_towns, teardown_ptr),
		cmocka_unit_test(vector_foreach_test),
		cmocka_unit_test(vector_transform_test),
	};

	return cmocka_run_group_tests(tests, setup_random, teardown_ptr);
}

