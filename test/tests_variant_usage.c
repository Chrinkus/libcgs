#include "cgs_test.h"

#include "cgs_variant.h"

struct point {
	int x;
	int y;
	int z;
};

int main(void)
{
	/*
	 * cgs_variant usage
	 *
	 * First create an empty variant..
	 */
	struct cgs_variant v1 = { 0 };
	/*
	 * Let's put an int into it..
	 */
	cgs_variant_set_int(&v1, 37);
	/*
	 * And read it back out..
	 */
	const int* pi = cgs_variant_get(&v1);
	/*
	 * Test to make sure its the same value..
	 */
	assert(*pi == 37);
	/*
	 * We can do the same thing with a double. Note that you probably won't
	 * be changing the type of your variants often, this just proves you
	 * can.
	 */
	cgs_variant_set_double(&v1, 5.973);
	const double* pd = cgs_variant_get(&v1);
	assert(*pd > 5.9);
	assert(*pd < 6.0);
	/*
	 * Strings are a little different. A string variant will always
	 * allocate a copy with strdup.
	 */
	const char buff[] = "Hello!";
	cgs_variant_set_string(&v1, buff);
	const char* s1 = cgs_variant_get(&v1);
	assert(strcmp(s1, buff) == 0);
	/*
	 * As such, string variants need to have their data freed.
	 */
	cgs_variant_free_data(&v1);
	/* 
	 * A dynamically allocated base string will need to be freed as well
	 * as the variant string.
	 */
	/*
	 * Setup..
	 */
	const char* static_string = "Allocated?";
	int len = strlen(static_string);
	char* alloc_string = malloc(len+1);
	strcpy(alloc_string, static_string);
	alloc_string[len-2] = '!';
	/*
	 * Now when the string variant is set there will be two allocated
	 * strings. The strings are the same but exist in two different
	 * allocations.
	 */
	cgs_variant_set_string(&v1, alloc_string);
	const char* s2 = cgs_variant_get(&v1);
	assert(strcmp(s2, alloc_string) == 0);
	assert(s2 != alloc_string);
	/*
	 * The caller is responsible for cleaning up the original string as
	 * well as the variant.
	 */
	free(alloc_string);
	cgs_variant_free_data(&v1);
	
	return EXIT_SUCCESS;
}

