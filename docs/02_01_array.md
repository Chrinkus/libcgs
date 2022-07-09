# Array

When the data quantities are not known at compile time we need a dynamic array that grows as needed.

## Memory Management

Arrays are allocated with `cgs_array_new` and deallocated with `cgs_array_free`. Both of these functions take a pointer to the array that you wish to manage as their first parameter. For the creation of an array we must also provide the size of our elements:

```
#include <cgs/cgs_array.h>

int main(void)
{
	struct cgs_array arr = { 0 };
	cgs_array_new(&arr, sizeof(int));
	
	// Use the array
	
	cgs_array_free(&arr);
}
```

## Adding Elements

Items can be added to the end of the array with `cgs_array_push`. The current number of elements in the array can be retrieved with `cgs_array_length`.

Here is a typical example where an unknown number of integers are read from input:

```
#include <stdio.h>
#include <cgs/cgs_array.h>

int main(void)
{
	/* Create the array as above */
	
	for (int n; scanf(" %d", &n) == 1; )
		cgs_array_push(&arr, &n);
		
	printf("%zu integers read\n", cgs_array_length(&arr));
	
	/* Don't forget to free! */
}
```

## Element Access

Accessing elements can be done in two ways, either by getting a pointer to an element with `cgs_array_get`:

```
	int positive_sum = 0;
	
	for (size_t i = 0; i < cgs_array_length(&arr); ++i) {
		const int* pi = cgs_array_get(&arr, i);
		if (*pi > 0)
			positive_sum += *pi;
	}
	
	printf("The sum of positive integers is %d\n", positive_sum);
```

Or by way of sub-scripting the array directly after a call to `cgs_array_data`:

```
	int negative_sum = 0;
	
	const int* ai = cgs_array_data(&arr);
	for (size_t i = 0; i < cgs_array_length(&arr); ++i)
		if (ai[i] < 0)
			negative_sum += ai[i];
			
	printf("The sum of negative integers is %d\n", negative_sum);
```

In both of the above examples we are accessing the elements through the const-by-default interface. For modifiable equivalents use `cgs_array_get_mutable` and `cgs_array_data_mutable`.

## Example Code

A listing of the example code complete with error handling can be found in the file `array_example.c`.

A file with integers can be found in the `data` folder of the project conveniently named `integers.txt`.

Compilation and test:
```
$ gcc array_example.c -lcgs
$ cat ../data/integers.txt | ./a.out
```

If you get a shared library error try:
```
$ gcc array_example.c -lcgs -Wl,-R/path/to/libcgs.so
```

The default path to `libcgs.so` on my machine is `/usr/local/lib64`.

