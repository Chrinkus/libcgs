#include "cmocka_headers.h"

#include "cgs_io.h"

#include <stdlib.h>	// free

const char* const data_path = "io_test_data.txt";

static int setup_file_content(void** state)
{
	(void)state;

	FILE* file = fopen(data_path, "w");

	if (!file)
		return -1;

	fputs("Birthday\n", file);
	fputs("Christmas\n", file);
	fputs("Tuesday\n", file);
	fputs("Books for reading\n", file);

	fclose(file);

	return 0;
}

static int setup_file_read(void** state)
{
	FILE* file = fopen(data_path, "r");

	if (!file)
		return -1;

	*state = file;

	return 0;
}

static int teardown_file_read(void** state)
{
	FILE* file = *(FILE**)state;
	fclose(file);
	return 0;
}

static void io_getline_test(void** state)
{
	FILE* file = *(FILE**)state;

        struct cgs_string buff = { 0 };
        assert_non_null(cgs_string_new(&buff));

	int n = cgs_io_getline(file, &buff);

	assert_int_equal(n, 8);
	assert_string_equal(cgs_string_data(&buff), "Birthday");

	cgs_string_clear(&buff);
	n = cgs_io_getline(file, &buff);
	assert_int_equal(n, 9);
	assert_string_equal(cgs_string_data(&buff), "Christmas");

	cgs_string_clear(&buff);
	n = cgs_io_getline(file, &buff);
	assert_int_equal(n, 7);
	assert_string_equal(cgs_string_data(&buff), "Tuesday");

	cgs_string_clear(&buff);
	n = cgs_io_getline(file, &buff);
	assert_int_equal(n, 17);
	assert_string_equal(cgs_string_data(&buff), "Books for reading");

	cgs_string_clear(&buff);
	n = cgs_io_getline(file, &buff);
	assert_int_equal(n, 0);
	assert_string_equal(cgs_string_data(&buff), "");

	cgs_string_free(&buff);
}

static void io_readline_test(void** state)
{
	FILE* file = *(FILE**)state;

	char* line = cgs_io_readline(file);
        assert_non_null(line);
	assert_string_equal(line, "Birthday");
	free(line);

	line = cgs_io_readline(file);
        assert_non_null(line);
	assert_string_equal(line, "Christmas");
	free(line);
	line = cgs_io_readline(file);
        assert_non_null(line);
	assert_string_equal(line, "Tuesday");
	free(line);
	line = cgs_io_readline(file);
        assert_non_null(line);
	assert_string_equal(line, "Books for reading");
	free(line);

	line = cgs_io_readline(file);
	assert_null(line);
}

static void io_readlines_test(void** state)
{
	FILE* file = *(FILE**)state;

        void* res = NULL;

        struct cgs_array lines = { 0 };
        res = cgs_array_new(&lines, sizeof(char*));
        assert_non_null(res);

        res = cgs_io_readlines(file, &lines);
        assert_non_null(res);
	assert_int_equal(cgs_array_length(&lines), 4);

	CgsStrIter b = cgs_array_begin(&lines);
	CgsStrIter e = cgs_array_end(&lines);
	assert_ptr_not_equal(b, e);

	assert_string_equal(*b++, "Birthday");
	assert_string_equal(*b++, "Christmas");
	assert_string_equal(*b++, "Tuesday");
	assert_string_equal(*b++, "Books for reading");
	assert_ptr_equal(b, e);

	cgs_array_free_all(&lines);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(io_getline_test,
				setup_file_read, teardown_file_read),
		cmocka_unit_test_setup_teardown(io_readline_test,
				setup_file_read, teardown_file_read),
		cmocka_unit_test_setup_teardown(io_readlines_test,
				setup_file_read, teardown_file_read),
	};

	return cmocka_run_group_tests(tests, setup_file_content, NULL);
}

