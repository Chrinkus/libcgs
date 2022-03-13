#include "cgs_io.h"
#include "cgs_test.h"

/* io_test_data.txt contents:
 * Birthday
 * Christmas
 * Tuesday
 * Books for reading
 *
 */

int io_getline_test(void* data)
{
	const char* path = (const char*)data;

	FILE* file = fopen(path, "r");

	struct cgs_string* buff = cgs_string_new();
	int n = cgs_io_getline(file, buff);

	assert(n == 8);
	assert(strcmp(cgs_string_read(buff), "Birthday") == 0);

	cgs_string_clear(buff);
	n = cgs_io_getline(file, buff);
	assert(n == 9);
	assert(strcmp(cgs_string_read(buff), "Christmas") == 0);

	cgs_string_clear(buff);
	n = cgs_io_getline(file, buff);
	assert(n == 7);
	assert(strcmp(cgs_string_read(buff), "Tuesday") == 0);

	cgs_string_clear(buff);
	n = cgs_io_getline(file, buff);
	assert(n == 17);
	assert(strcmp(cgs_string_read(buff), "Books for reading") == 0);

	cgs_string_clear(buff);
	n = cgs_io_getline(file, buff);
	assert(n == 0);
	assert(strcmp(cgs_string_read(buff), "") == 0);

	cgs_string_free(buff);
	fclose(file);

	return TEST_SUCCESS;
}

int io_readline_test(void* data)
{
	const char* path = (const char*)data;
	FILE* file = fopen(path, "r");

	char* line = cgs_io_readline(file);
	assert(strcmp(line, "Birthday") == 0);
	free(line);

	line = cgs_io_readline(file);
	assert(strcmp(line, "Christmas") == 0);
	free(line);
	line = cgs_io_readline(file);
	assert(strcmp(line, "Tuesday") == 0);
	free(line);
	line = cgs_io_readline(file);
	assert(strcmp(line, "Books for reading") == 0);
	free(line);

	line = cgs_io_readline(file);
	assert(line == NULL);

	fclose(file);

	return TEST_SUCCESS;
}

int io_readlines_test(void* data)
{
	const char* path = (const char*)data;
	FILE* file = fopen(path, "r");

	struct cgs_array* lines = cgs_io_readlines(file);
	assert(cgs_array_length(lines) == 4);

	CgsStrIter b = cgs_array_begin(lines);
	CgsStrIter e = cgs_array_end(lines);
	assert(b != e);

	assert(strcmp(*b++, "Birthday") == 0);
	assert(strcmp(*b++, "Christmas") == 0);
	assert(strcmp(*b++, "Tuesday") == 0);
	assert(strcmp(*b++, "Books for reading") == 0);
	assert(b == e);

	cgs_array_free_all(lines);
	fclose(file);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "io_getline", io_getline_test, "io_test_data.txt" },
		{ "io_readline", io_readline_test, "io_test_data.txt" },
		{ "io_readlines", io_readlines_test, "io_test_data.txt" },
	};

	return cgs_run_tests(tests);
}

