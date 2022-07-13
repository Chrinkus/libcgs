# String

A dynamic string implementation.

```C
#include <cgs/cgs_string.h>

struct cgs_string {
	size_t length;
	size_t capacity;
	char* data;
};
```

## String API

|Function|What it does|
|---|---|
|`cgs_string_new`|Allocates memory for a dynamic string. String is initially empty.|
|`cgs_string_new_from_string`|Allocates enough memory for a new string that is a copy of the provided standard string|
|`cgs_string_free`|Frees the memory of a string.|
|`cgs_string_xfer`|Releases ownership of a string by returning a `char*` to the internal data.|
|`cgs_string_length`|Get the length of the string.|
|`cgs_string_data`|Provides read-only access to the inner data through a `const char*`.|
|`cgs_string_data_mutable`|Provides writable access to the inner data through a `char*`.|
|`cgs_string_push`|Append a character to a string.|
|`cgs_string_clear`|Reset a string to empty and set length to 0. Allocated memory capacity is unchanged.|
|`cgs_string_erase`|Reset a string to empty and set length to 0. All previous string characters are overwritten with '\0'. Allocated memory capacity is unchanged.|
|`cgs_string_sort`|Sorts the characters of the string in-place.|

### A Word About Regular C-Strings

I actually like C-strings. Whether we're talking `char*` or `char[]` I prefer to use the standard strings whenever I can. However, there are times when I'm assembling a string from unknown input one character at a time or by concatenating several strings together. This dynamic string implementation is meant to aid in those circumstances by handling all of the allocation and growth management.

After your dynamic needs are met you are free to shrink and transfer ownership out of the string object to a `char*` for regular use.

## Memory Management

Strings are allocated with `cgs_string_new` and freed with `cgs_string_free`. Both of these functions take a pointer to the string that you wish to manage as their first parameter.

```C
struct cgs_string s = { 0 };
cgs_string_new(&s);

// Use the string

cgs_string_free(&s);
```

Strings can also be initialized from another regular string with `cgs_string_new_from_string`.

```C
struct cgs_string s = { 0 };
cgs_string_new_from_string(&s, "C wears well");
```

## Adding Characters

Characters can be added to the end of the string one at a time with `cgs_string_push`. The current number of characters in the string can be retrieved in O(1) time with `cgs_string_length`.

Here is an example of an unknown line of text being read from input:

```C
#include <stdio.h>
#include <cgs/cgs_string.h>

int main(void)
{
	/* Create the string as shown above */
	
	for (int c; (c = getchar()) != EOF && c != '\n'; )
		cgs_string_push(&s, c);
		
	printf("%zu characters read\n", cgs_string_length(&s));
	
	/* Don't forget to free! */
}
```

## Character Access