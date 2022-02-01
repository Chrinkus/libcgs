#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

enum test_constants {
	TEST_SUCCESS = 0,
	TEST_FAILURE = 1,
	TEST_SKIP = 77,
};

typedef int (*cgs_test_func)(void*);

struct test {
	const char* name;
	cgs_test_func func;
	void* data;
};

#define ARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

#define cgs_run_tests(arr) run_tests((arr), ARR_SIZE(arr))

int run_tests(struct test* tests, int num_tests)
{
	int ret = TEST_SUCCESS;
	for (int i = 0; i < num_tests; ++i) {
		printf("Checking %s... ", tests[i].name);

		int rc = tests[i].func(tests[i].data);
		switch (rc) {
		case TEST_SUCCESS:	printf("PASS\n");	break;
		case TEST_SKIP:		printf("SKIP\n");	break;
		case TEST_FAILURE:
			printf("FAIL\n");
			ret = rc;
			break;
		}
	}
	return ret;
}

