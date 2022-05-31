/* cgs_test.h
 *
 * MIT License
 * 
 * Copyright (c) 2022 Chris Schick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

