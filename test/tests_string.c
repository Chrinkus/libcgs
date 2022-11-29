#include "cmocka_headers.h"

#include "cgs_string.h"

static void
string_new_test(void** state)
{
	(void)state;

        struct cgs_string s = cgs_string_new();

	assert_int_equal(s.length, 0);
        assert_int_equal(s.capacity, 0);
        assert_null(s.data);
}

static void
string_from_test(void** state)
{
        (void)state;
	const char* test = "Super test string";

        struct cgs_string s1 = cgs_string_new();
        assert_non_null(cgs_string_from(test, &s1));

	assert_int_equal(cgs_string_length(&s1), strlen(test));
	assert_string_equal(cgs_string_data(&s1), test);
	assert_ptr_not_equal(cgs_string_data(&s1), test);

	cgs_string_push(&s1, '!');
	assert_string_not_equal(cgs_string_data(&s1), test);

	cgs_string_free(&s1);
}

/*
int string_xfer_test(void* data)
{
	const char* test = (const char*)data;

        struct cgs_string s = { 0 };
        cgs_string_new_from_str(&s, test);

	char* p = cgs_string_xfer(&s);

	assert(strcmp(p, test) == 0);
	assert(p != test);

	free(p);

	return TEST_SUCCESS;
}
*/

static void
string_push_test(void** state)
{
	(void)state;

        struct cgs_string s = cgs_string_new();
	assert_non_null(cgs_string_push(&s, 'p'));
	assert_non_null(cgs_string_push(&s, 'u'));
	assert_non_null(cgs_string_push(&s, 's'));
	assert_non_null(cgs_string_push(&s, 'h'));

	assert_int_equal(s.length, 4);
	assert_string_equal(s.data, "push");

	cgs_string_free(&s);
}

/*
int string_clear_test(void* data)
{
	(void)data;

        struct cgs_string s = { 0 };
	cgs_string_new_from_str(&s, "Howdy");

	cgs_string_clear(&s);

	assert(cgs_string_length(&s) == 0);
	assert(cgs_string_data(&s) != NULL);

	// 'clear' just changes first char to '\0'
	assert(cgs_string_data(&s)[1] == 'o');
	assert(cgs_string_data(&s)[2] == 'w');
	assert(cgs_string_data(&s)[3] == 'd');
	assert(cgs_string_data(&s)[4] == 'y');
	assert(cgs_string_data(&s)[5] == '\0');

	cgs_string_free(&s);

	return TEST_SUCCESS;
}

int string_erase_test(void* data)
{
	(void)data;

        struct cgs_string s = { 0 };
	cgs_string_new_from_str(&s, "Howdy");

	cgs_string_erase(&s);

	assert(cgs_string_length(&s) == 0);
	assert(cgs_string_data(&s) != NULL);

	// 'erase' overwrites all characters with '\0'
	assert(cgs_string_data(&s)[1] == '\0');
	assert(cgs_string_data(&s)[2] == '\0');
	assert(cgs_string_data(&s)[3] == '\0');
	assert(cgs_string_data(&s)[4] == '\0');
	assert(cgs_string_data(&s)[5] == '\0');

	cgs_string_free(&s);

	return TEST_SUCCESS;
}

int string_sort_test(void* data)
{
	(void)data;

        struct cgs_string s = { 0 };
	cgs_string_new_from_str(&s, "abcdabcdabcd");

	cgs_string_sort(&s);

	assert(strcmp(cgs_string_data(&s), "aaabbbcccddd") == 0);

	cgs_string_free(&s);

	return TEST_SUCCESS;
}

int string_prepend_test(void* data)
{
        (void)data;

        struct cgs_string s1 = { 0 };
        cgs_string_new_from_str(&s1, "Feature");

        struct cgs_string s2 = { 0 };
        cgs_string_new_from_str(&s2, "Creature ");
        assert(cgs_string_prepend(&s1, &s2) != NULL);

        assert(cgs_string_length(&s1) == strlen("Creature Feature"));
        assert(strcmp(cgs_string_data(&s1), "Creature Feature") == 0);

        assert(cgs_string_length(&s2) == strlen("Creature "));
        assert(strcmp(cgs_string_data(&s2), "Creature ") == 0);

        cgs_string_free(&s1);
        cgs_string_free(&s2);
        return TEST_SUCCESS;
}

int string_append_test(void* data)
{
        (void)data;
        struct cgs_string s1 = { 0 };
        cgs_string_new_from_str(&s1, "Colorado");

        struct cgs_string s2 = { 0 };
        cgs_string_new_from_str(&s2, " Avalanche");
        assert(cgs_string_append(&s1, &s2) != NULL);
        assert(cgs_string_length(&s1) == strlen("Colorado Avalanche"));
        assert(strcmp(cgs_string_data(&s1), "Colorado Avalanche") == 0);

        assert(cgs_string_length(&s2) == strlen(" Avalanche"));
        assert(strcmp(cgs_string_data(&s2), " Avalanche") == 0);

        cgs_string_free(&s1);
        cgs_string_free(&s2);
        return TEST_SUCCESS;
}

int string_end_test(void* data)
{
        (void)data;

        struct cgs_string s1 = { 0 };
        cgs_string_new_from_str(&s1, "Hello World");

        const char* p = cgs_string_end(&s1);
        assert(*p == '\0');
        --p;
        assert(*p == 'd');
        --p;
        assert(*p == 'l');

        cgs_string_free(&s1);

        return TEST_SUCCESS;
}
*/

int main(void)
{
	// struct test tests[] = {
	// 	{ "string_new", string_new_test, NULL },
	// 	{ "string_xfer", string_xfer_test, "Transfer me!" },
	// 	{ "string_push", string_push_test, NULL },
	// 	{ "string_clear", string_clear_test, NULL },
	// 	{ "string_erase", string_erase_test, NULL },
	// 	{ "string_new_from_str", string_new_from_str_test,
	// 		"Super test string" },
	// 	{ "string_sort", string_sort_test, NULL },
	// 	{ "string_prepend", string_prepend_test, NULL },
	// 	{ "string_append", string_append_test, NULL },
	// 	{ "string_end", string_end_test, NULL },
	// };

	// return cgs_run_tests(tests);
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(string_new_test),
                cmocka_unit_test(string_from_test),
                cmocka_unit_test(string_push_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
