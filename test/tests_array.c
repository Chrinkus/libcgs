#include "cgs_array.h"
#include "cgs_test.h"

#include "cgs_compare.h"
#include "cgs_string_utils.h"

int test_int_10[10] = { 37, 2, 10, -13, -22, 5, 18, -1, 0, 30 };
int test_int_iota[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

int array_new_test(void* data)
{
	(void)data;

	struct cgs_array* ai = cgs_array_new(int);

	assert(ai != NULL);
	assert(cgs_array_length(ai) == 0);

	cgs_array_free(ai);

	struct cgs_array* ad = cgs_array_new(double);
	assert(ad != NULL);
	assert(cgs_array_length(ad) == 0);

	cgs_array_free(ad);

	struct cgs_array* as = cgs_array_new(char*);
	assert(as != NULL);
	assert(cgs_array_length(as) == 0);

	cgs_array_free(as);

	return TEST_SUCCESS;
}

int array_push_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new(int);
	for (int i = 0; i < 10; ++i)
		cgs_array_push(ai, &ints[i]);

	assert(cgs_array_length(ai) == 10);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_sort_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	cgs_array_sort(ai, cgs_int_cmp);

	int min = *(int*)cgs_array_get(ai, 0);
	int max = *(int*)cgs_array_get(ai, 9);
	assert(min == -22);
	assert(max == 37);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_find_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	int x = 99;
	int* found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert(found == NULL);
	x = -13;
	found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert(found != NULL);
	assert(found != &x);
	assert(*found == -13);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_xfer_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	size_t len = 0;
	int* out = cgs_array_xfer(ai, &len);

	assert(len == 10);
	assert(out != NULL);
	assert(out != ints);

	for (size_t i = 0; i < len; ++i)
		assert(out[i] == ints[i]);

	free(out);

	return TEST_SUCCESS;
}

int array_iter_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	const int* b = cgs_array_begin(ai);
	assert(b != NULL);

	const int* e = cgs_array_end(ai);
	assert(e != NULL);

	int sum = 0;
	for ( ; b != e; ++b)
		sum += *b;

	assert(sum == 55);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_new_from_array_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	assert(ai != NULL);
	assert(cgs_array_length(ai) == 10);

	for (size_t i = 0; i < cgs_array_length(ai); ++i)
		assert(*(int*)cgs_array_get(ai, i) == ints[i]);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_strings_test(void* data)
{
	(void)data;

	const char* const strs[5] = {
		"Zora",
		"Anola",
		"Hazelridge",
		"Oakbank",
		"Dugald",
	};

	// New
	struct cgs_array* as = cgs_array_new(char*);
	// Push
	for (int i = 0; i < 5; ++i) {
		char* p = cgs_strdup(strs[i]);
		cgs_array_push(as, &p);
	}
	assert(cgs_array_length(as) == 5);

	// Get :(
	const char*const* p = cgs_array_get(as, 0);
	assert(strcmp(*p, strs[0]) == 0);

	// Iterators :)
	CgsStrIter b = cgs_array_begin(as);
	CgsStrIter e = cgs_array_end(as);
	for (CgsStrIter s = strs; b != e; ++b, ++s)
		assert(strcmp(*b, *s) == 0);

	// Free
	cgs_array_free_with_data(as);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "array_new", array_new_test, NULL },
		{ "array_push", array_push_test, test_int_10 },
		{ "array_sort", array_sort_test, test_int_10 },
		{ "array_find", array_find_test, test_int_10 },
		{ "array_xfer", array_xfer_test, test_int_10 },
		{ "array_new_from_array", array_new_from_array_test,
			test_int_10 },
		{ "array_strings", array_strings_test, NULL },
		{ "array_iter", array_iter_test, test_int_iota },
	};

	return cgs_run_tests(tests);
}

