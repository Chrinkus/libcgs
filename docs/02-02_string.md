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
|`cgs_string_new`|Allocates memory for a dynamic string struct. String is initially empty.|
|`cgs_string_new_from_str`|Allocates enough memory for a new string struct that is a copy of the provided standard string|
|`cgs_string_free`|Frees the memory of a string.|
|`cgs_string_shrink`|Reduces the allocated memory of a string struct to the minimum required for the inner string.|
|`cgs_string_xfer`|Releases ownership of a string by returning a `char*` to the internal data.|
|`cgs_string_length`|Get the length of the string.|
|`cgs_string_data`|Provides read-only access to the inner data through a `const char*`.|
|`cgs_string_data_mutable`|Provides writable access to the inner data through a `char*`.|
|`cgs_string_get`|Gets a read-only pointer to an index within the string. No bounds checking.|
|`cgs_string_char`|Gets the character at an index within a string. No bounds checking.|
|`cgs_string_push`|Append a character to a string.|
|`cgs_string_prepend`|Insert the contents of a dynamic string to the front of another.|
|`cgs_string_append`|Add the contents of a dynamic string to the end of another.|
|`cgs_string_prepend_str`|Insert the contents of a standard string to the front of a string struct.|
|`cgs_string_append_str`|Add the contents of a standard string to the end of a string struct.|
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

Strings can also be initialized from another regular string with `cgs_string_new_from_str`: 

```C
struct cgs_string s = { 0 };
cgs_string_new_from_str(&s, "Hello string!");
```

Note that this is consistent with the library's general idiom of referring to standard strings with the shortened `str`.

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

## Joining Strings

Strings can be combined using the `cgs_string_append` and `cgs_string_prepend` group of functions. In this form these functions combine two `struct cgs_string` objects:

```C
struct cgs_string s1 = { 0 };
cgs_string_new_from_str(&s1, "Peanut butter");
struct cgs_string s2 = { 0 };
cgs_string_new_from_str(&s2, " and jelly");

cgs_string_append(&s1, &s2);	// s1 == "Peanut butter and jelly"

struct cgs_string s3 = { 0 };
cgs_string_new_from_str(&s3, "Jam");

cgs_string_prepend(&s2, &s3);	// s2 == "Jam and jelly"
```

These functions can take standard strings as their second argument by calling them with a `_str` suffix. Here both are used to construct a quote from K&R:

```C
struct cgs_string s1 = { 0 };
cgs_string_new_from_str(&s1, "and it wears well");

cgs_string_append_str(&s1, " as one's experience with it grows.");

cgs_string_prepend_str(&s1, "C is easy to learn, ");
```


## Character Access

Accessing characters within a string can be done in a few different ways. To view a character at any position in the string use `cgs_string_char`. To get a read-only pointer to a character use `cgs_string_get`:

```C
struct cgs_string s = { 0 };
cgs_string_new_from_str(&s, "purple");

char c = cgs_string_char(&s, 2);	// c == 'r'
const char* p = cgs_string_get(&s, 4);	// *p == 'l'
```

Subscript access can be performed after retrieving a read-only pointer to the inner string with `cgs_string_data`:

```C
struct cgs_string s = { 0 };
cgs_string_new_from_str(&s, "Resident Evil");

const char* p = cgs_string_data(&s);	// p[0] == 'R', p[1] == 'e', etc
```

In the cases of `_get` and `_data` we are accessing the characters through the const-by-default interface. For modifiable equivalents use `cgs_string_get_mutable` and `cgs_string_data_mutable`.