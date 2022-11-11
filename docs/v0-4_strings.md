# Strings
#### Version 0.4 Intentions

So far this library implements a dynamic string and some utilities for interacting with regular C-strings. For version 0.4 I would like to develop a wider view on the interactions between strings and other library features as well as introduce a sub-string struct.

## Sub-Strings

A sub-string struct will serve a similar purpose to string views in C++ and string slices in Rust. It will have the following characteristics:

* Non-owning
* Smaller footprint than dynamic string
* Not allocated
* Not null-terminated

## Purpose

The initial conception was based off a desire to include string-splitting in the library. Early drafts had new dynamic strings being allocated for each splitted sub-string. This solution had two downsides in extra allocations as well as a heavier memory cost.

The sub-string solution uses a third less memory than dynamic strings and does not allocate a copy of the sub-string. Instead it uses a "pointer-comma-length" approach that references a piece of the original string.

## Points of Concern

### Pointer Problems

Since pointers are being stored into memory owned by another object there are a few familiar concerns that have to be appreciated.

* Any 'push' into the target string may cause a reallocation and invalidate the pointer contained within a sub-string.
    - This will be up to the programmer to not attempt to create sub-strings while still building the target string.
* Sub-string life-times are tied to the original object. We cannot return a sub-string of a local string from a function.
    - A `to_string` utility function will be supplied for when we find a sub-string that we like so much we need to keep it.

### Printing Problems

Sub-strings are not C-strings, they are not null-terminated. As such, attempts to directly print the pointer within will result in the sub-string plus the remainder of the original string.

Solutions to this include:
* Writing a 'print' utility function to print sub-strings.
* Utilizing `printf`'s precision functionality with the `len` member of the struct to print that many characters.

### Mutability

Most uses of sub-strings do not require write-access to the target string. If there is a desire to alter the sub-string it is probably best to use the previously mentioned `to_string()` functionality to create a copy that can then be mutated to suit whatever needs arise.

## Interactions With Library

A string-split will result in a dynamic array of sub-strings. The string being split could possibly be the result of an IO read. The following illustrates an ideal sequence of calls w/o error checking:

```C
struct cgs_string buff = cgs_string_new();
cgs_io_getline(stdin, &buff);

struct cgs_array subs = cgs_array_new(sizeof(struct cgs_strsub));
cgs_string_split(&buff, ' ', &subs);

// do something with the sub-strings
```

_Note that the above models other usability changes that are planned for the library and does not represent the library as it currently exists_

## Implementation Notes

This is the likely struct definition:

```C
struct cgs_strsub {
        const char* sub;    // A pointer into another string
        size_t len;         // The length of the sub-string
};
```

Constructors:

```C
cgs_strsub_new(const char*, size_t);
cgs_strsub_from(const char*);
```

Adapters:

```C
struct cgs_strsub   // no allocation, return value
cgs_string_to_strsub(const struct cgs_string*);

void*               // allocation, potential fail, provide out-param
cgs_strsub_to_string(const struct cgs_strsub*, struct cgs_string*);
```

There will most likely need to be three regular `_split()` functions:

```C
void*
cgs_str_split(const char*, char, struct cgs_array*);

void*
cgs_string_split(const struct cgs_string*, char, struct cgs_array*);

void*
cgs_strsub_split(const struct cgs_strsub*, char, struct cgs_array*);
```

#### Source Code Organization

The above 'split' scenario suggests that, while separate headers should be kept, all implementation code could be in the same, shared 'cgs\_string.c' file.
