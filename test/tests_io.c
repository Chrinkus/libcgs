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

int main(void)
{
	struct test tests[] = {
		{ "io_getline", io_getline_test, "io_test_data.txt" },
	};

	return cgs_run_tests(tests);
}

