#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define ARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

enum test_constants { TEST_SUCCESS, TEST_FAILURE };

typedef int (*test_func)(void*);

struct test {
	const char* name;
	test_func func;
	void* data;
};

int run_tests(struct test* tests, int num_tests)
{
	for (int i = 0; i < num_tests; ++i) {
		printf("Checking %s... ", tests[i].name);
		int rc = tests[i].func(tests[i].data);
		if (rc != TEST_SUCCESS) {
			printf("FAIL\n");
			exit(rc);
		} else {
			printf("PASS\n");
		}
	}
	return TEST_SUCCESS;
}
