#include "cmocka_headers.h"

#include <stdlib.h>	// malloc, free

#include "cgs_array.h"

#include "cgs_compare.h"
#include "cgs_string_utils.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Setups and teardowns
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum { NUM_RANDOMS = 10, NUM_IOTAS = 10, NUM_TOWNS = 5 };

static int setup_random(void** state)
{
	int* ai = malloc(sizeof(int) * NUM_RANDOMS);
	if (!ai)
		return -1;
	ai[0] = 37;
	ai[1] = 2;
	ai[2] = 10;
	ai[3] = -13;
	ai[4] = -22;
	ai[5] = 5;
	ai[6] = 18;
	ai[7] = -1;
	ai[8] = 0;
	ai[9] = 30;

	*state = ai;

	return 0;
}

static int setup_iota(void** state)
{
	int* ai = malloc(sizeof(int) * NUM_IOTAS);
	if (!ai)
		return -1;

	for (int i = 0, n = 1; i < NUM_IOTAS; ++i, ++n)
		ai[i] = n;

	*state = ai;

	return 0;
}

static int setup_towns(void** state)
{
	const char** as = malloc(sizeof(char*) * NUM_TOWNS);
	if (!as)
		return -1;

	as[0] = "Cooks Creek";
	as[1] = "Anola";
	as[2] = "Hazelridge";
	as[3] = "Oakbank";
	as[4] = "Dugald";

	*state = as;

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

static void array_new_test(void** state)
{
	(void)state;

        struct cgs_array ai = { 0 };
        void* res = cgs_array_new(&ai, sizeof(int));
	//struct cgs_array* ai = cgs_array_new(int);

	assert_non_null(res);
	assert_int_equal(cgs_array_length(&ai), 0);

	cgs_array_free(&ai);

        struct cgs_array ad = { 0 };
        res = cgs_array_new(&ad, sizeof(double));
	//struct cgs_array* ad = cgs_array_new(double);
	assert_non_null(res);
	assert_int_equal(cgs_array_length(&ad), 0);

	cgs_array_free(&ad);

        struct cgs_array as = { 0 };
        res = cgs_array_new(&as, sizeof(char*));
	//struct cgs_array* as = cgs_array_new(char*);
	assert_non_null(res);
	assert_int_equal(cgs_array_length(&as), 0);

	cgs_array_free(&as);
}

static void array_push_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        cgs_array_new(&ai, sizeof(int));
	//struct cgs_array* ai = cgs_array_new(int);
	for (int i = 0; i < NUM_RANDOMS; ++i)
		cgs_array_push(&ai, &ints[i]);

	assert_int_equal(cgs_array_length(&ai), NUM_RANDOMS);

	cgs_array_free(&ai);
}

static void array_remove_test(void** state)
{
        (void)state;

        struct cgs_array a = { 0 };
        cgs_array_new(&a, sizeof(int));

        for (int i = 1; i <= 10; ++i)
                cgs_array_push(&a, &i);

        assert_int_equal(cgs_array_length(&a), 10);

        const int* pi = cgs_array_get(&a, 5);
        assert_int_equal(*pi, 6);

        cgs_array_remove(&a, 5);
        assert_int_equal(cgs_array_length(&a), 9);
        pi = cgs_array_get(&a, 5);
        assert_int_equal(*pi, 7);

        cgs_array_remove(&a, 0);
        assert_int_equal(cgs_array_length(&a), 8);
        pi = cgs_array_get(&a, 5);
        assert_int_equal(*pi, 8);
        pi = cgs_array_get(&a, 0);
        assert_int_equal(*pi, 2);

        cgs_array_remove(&a, 7);
        assert_int_equal(cgs_array_length(&a), 7);
        pi = cgs_array_get(&a, 5);
        assert_int_equal(*pi, 8);

        cgs_array_free(&a);
}

static void array_remove_fast_test(void** state)
{
        (void)state;

        struct cgs_array a = { 0 };
        cgs_array_new(&a, sizeof(int));

        for (int i = 1; i <= 10; ++i)
                cgs_array_push(&a, &i);

        assert_int_equal(cgs_array_length(&a), 10);

        const int* pi = cgs_array_get(&a, 4);
        assert_int_equal(*pi, 5);

        cgs_array_remove_fast(&a, 4);
        assert_int_equal(cgs_array_length(&a), 9);
        pi = cgs_array_get(&a, 4);
        assert_int_equal(*pi, 10);

        cgs_array_remove_fast(&a, 4);
        assert_int_equal(cgs_array_length(&a), 8);
        pi = cgs_array_get(&a, 4);
        assert_int_equal(*pi, 9);

        cgs_array_remove_fast(&a, 7);
        assert_int_equal(cgs_array_length(&a), 7);
        pi = cgs_array_get(&a, 4);
        assert_int_equal(*pi, 9);

        cgs_array_free(&a);
}

static void array_clear_test(void** state)
{
        (void)state;

        struct cgs_array ai = { 0 };
        cgs_array_new(&ai, sizeof(int));
        
        for (int i = 1; i <= 10; ++i)
                cgs_array_push(&ai, &i);

        const int* data1 = cgs_array_data(&ai);
        size_t cap1 = ai.capacity;

        assert_int_equal(cgs_array_length(&ai), 10);
        cgs_array_clear(&ai);
        assert_int_equal(cgs_array_length(&ai), 0);

        const int* data2 = cgs_array_data(&ai);
        size_t cap2 = ai.capacity;

        assert_ptr_equal(data1, data2);
        assert_int_equal(cap1, cap2);
}

static void array_sort_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        cgs_array_new_from_array(&ai, sizeof(ints[0]), ints, NUM_RANDOMS);
        /*
	struct cgs_array* ai = cgs_array_new_from_array(ints, NUM_RANDOMS,
			sizeof(int));
        */

	cgs_array_sort(&ai, cgs_int_cmp);

	int min = *(int*)cgs_array_get(&ai, 0);
	int max = *(int*)cgs_array_get(&ai, 9);
	assert_int_equal(min, -22);
	assert_int_equal(max, 37);

	cgs_array_free(&ai);
}

static void array_find_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        cgs_array_new_from_array(&ai, sizeof(ints[0]), ints, NUM_RANDOMS);

	int x = 99;
	int* found = cgs_array_find(&ai, cgs_int_pred, &x);
	assert_null(found);
	x = -13;
	found = cgs_array_find(&ai, cgs_int_pred, &x);
	assert_non_null(found);
	assert_ptr_not_equal(found, &x);
	assert_int_equal(*found, -13);

	cgs_array_free(&ai);
}

static void array_xfer_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        cgs_array_new_from_array(&ai, sizeof(ints[0]), ints, NUM_RANDOMS);
        /*
	struct cgs_array* ai = cgs_array_new_from_array(ints, NUM_RANDOMS,
			sizeof(int));
        */

	size_t len = 0;
	int* out = cgs_array_xfer(&ai, &len);

	assert_int_equal(len, 10);
	assert_non_null(out);
	assert_ptr_not_equal(out, ints);

	for (size_t i = 0; i < len; ++i)
		assert_int_equal(out[i], ints[i]);

	free(out);
}

static void array_copy_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array a1 = { 0 };
        cgs_array_new(&a1, sizeof(int));

	for (int i = 0; i < NUM_RANDOMS; ++i)
		cgs_array_push(&a1, &ints[i]);

        struct cgs_array a2 = { 0 };
        assert_non_null(cgs_array_copy(&a2, &a1));
        assert_int_not_equal(a2.length, 0);
        assert_int_not_equal(a2.capacity, 0);
        assert_int_not_equal(a2.element_size, 0);
        assert_non_null(a2.data);

        assert_int_equal(a2.length, a1.length);
        assert_int_equal(a2.capacity, a1.capacity);
        assert_int_equal(a2.element_size, a1.element_size);
        assert_ptr_not_equal(a2.data, a1.data);

        for (size_t i = 0; i < cgs_array_length(&a1); ++i) {
                const int* p1 = cgs_array_get(&a1, i);
                const int* p2 = cgs_array_get(&a2, i);
                assert_int_equal(*p1, *p2);
        }

        cgs_array_free(&a1);
        cgs_array_free(&a2);
}

static void array_new_from_array_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        void* res = cgs_array_new_from_array(&ai, sizeof(ints[0]), ints,
                        NUM_RANDOMS);
        assert_non_null(res);
        /*
	struct cgs_array* ai = cgs_array_new_from_array(ints, NUM_RANDOMS,
			sizeof(int));
        */

	assert_int_equal(cgs_array_length(&ai), NUM_RANDOMS);

	for (size_t i = 0; i < cgs_array_length(&ai); ++i)
		assert_int_equal(*(int*)cgs_array_get(&ai, i), ints[i]);

	cgs_array_free(&ai);
}

static void array_iter_test(void** state)
{
	const int* ints = *(const int**)state;

        struct cgs_array ai = { 0 };
        cgs_array_new_from_array(&ai, sizeof(ints[0]), ints, NUM_IOTAS);
        /*
	struct cgs_array* ai = cgs_array_new_from_array(ints, NUM_IOTAS,
			sizeof(int));
        */

	const int* b = cgs_array_begin(&ai);
	assert_non_null(b);

	const int* e = cgs_array_end(&ai);
	assert_non_null(e);

	int sum = 0;
	while (b != e)
		sum += *b++;

	assert_int_equal(sum, 55);

	cgs_array_free(&ai);
}

static void array_strings_test(void** state)
{
	const char** strs = *(const char***)state;

	// New
        struct cgs_array as = { 0 };
        cgs_array_new(&as, sizeof(char*));
	//struct cgs_array* as = cgs_array_new(char*);
	// Push
	for (int i = 0; i < NUM_TOWNS; ++i) {
		char* p = cgs_strdup(strs[i]);
		cgs_array_push(&as, &p);
	}

	assert_int_equal(cgs_array_length(&as), NUM_TOWNS);

	// Get :(
	const char*const* p = cgs_array_get(&as, 0);
	assert_string_equal(*p, strs[0]);

	// Iterators :)
	CgsStrIter b = cgs_array_begin(&as);
	CgsStrIter e = cgs_array_end(&as);
	for (CgsStrIter s = strs; b != e; ++b, ++s)
		assert_string_equal(*b, *s);

	cgs_array_free_all(&as);
}

void accum_int(const void* p, size_t i, void* data)
{
        int val = *(const int*)p;
        int* sum = data;
        (void)i;

        *sum += val;
}

static void array_foreach_test(void** state)
{
        (void)state;

        struct cgs_array ai = { 0 };
        cgs_array_new(&ai, sizeof(int));
        //struct cgs_array* ai = cgs_array_new(int);

        for (int i = 0; i < 10; ++i)
                cgs_array_push(&ai, &i);

        int sum = 0;
        cgs_array_foreach(&ai, accum_int, &sum);

        assert_int_equal(sum, 45);

        cgs_array_free(&ai);
}

void double_int(void* ele, size_t i, void* data)
{
        int* p = ele;
        (void)i;
        (void)data;

        *p *= 2;
}

static void array_transform_test(void** state)
{
        (void)state;

        struct cgs_array ai = { 0 };
        cgs_array_new(&ai, sizeof(int));
        //struct cgs_array* ai = cgs_array_new(int);

        for (int i = 0; i < 10; ++i)
                cgs_array_push(&ai, &i);

        cgs_array_transform(&ai, double_int, NULL);

        const int* p = cgs_array_get(&ai, 0);
        assert_int_equal(p[0], 0);
        assert_int_equal(p[1], 2);
        assert_int_equal(p[5], 10);
        assert_int_equal(p[9], 18);

        cgs_array_free(&ai);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(array_new_test),
		cmocka_unit_test(array_push_test),
		cmocka_unit_test(array_remove_test),
		cmocka_unit_test(array_remove_fast_test),
                cmocka_unit_test(array_clear_test),
		cmocka_unit_test(array_sort_test),
		cmocka_unit_test(array_find_test),
		cmocka_unit_test(array_xfer_test),
		cmocka_unit_test(array_copy_test),
		cmocka_unit_test(array_new_from_array_test),
		cmocka_unit_test_setup_teardown(array_iter_test, setup_iota,
				teardown_ptr),
		cmocka_unit_test_setup_teardown(array_strings_test,
				setup_towns, teardown_ptr),
		cmocka_unit_test(array_foreach_test),
		cmocka_unit_test(array_transform_test),
	};

	return cmocka_run_group_tests(tests, setup_random, teardown_ptr);
}

