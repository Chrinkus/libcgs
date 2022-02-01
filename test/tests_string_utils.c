#include "cgs_string_utils.h"
#include "test_utils.h"

int strdup_test(void* data)
{
	const char* s1 = data;
	char* s2 = cgs_strdup(s1);

	assert(s2 != NULL);
	assert(s2 != s1);
	assert(strcmp(s1, s2) == 0);

	free(s2);

	const char* s3 = "";
	char* s4 = cgs_strdup(s3);

	assert(s3 != NULL);
	assert(s4 != s3);
	assert(strcmp(s3, s4) == 0);

	free(s4);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "strdup", strdup_test, "Ninja doesn't like gnu_strdup" },
	};

	return run_tests(tests, ARR_SIZE(tests));
}

