# Library Overview

#### Change Log

_2022-07-09_

Created document to state project direction and define idioms.

## Guiding Need

This library of data structures in C has been created for and driven by the problems in the Advent of Code programming challenges (specifically the 2018 set of problems). As challenges require new data structures they have been added to the library.

As an example, day 1 of the 2018 set requires the looped reading of a fixed set of input integers. The number of inputs would only be known at run-time so a dynamic array was needed. This would be the first data structure added to the library.

In examining solutions to problems I have found the need to tweak implementations and even been forced to add a new data structure that better suits the problem domain. In adding new data structures there has been a desire to standardize the names of operations. This has led to breaking changes for previous solutions and required rewriting of otherwise "complete" code.

Much has been learned.

## Development Environment

This library is currently being developed on Fedora Linux 36. Initially the project began on an Ubuntu 21.10 installation.

|Tool|Software|Version|
|---|---|---|
|Operating System|Fedora|36|
|Kernel|Linux|5.18|
|Compiler|GCC|12.1|
|Language|C|C99|
|Build|CMake|3.22*|

\* cmake_minimum_required for project set to 3.18


## Testing Practices

Unit tests are written using CMocka and run using the CMake sub-tool CTest. After tests have passed they are run through Valgrind to detect memory leaks.

## Naming Conventions

All public structures, enumerations and functions are namespaced with `cgs_`. Data structure names are represented by a single word as much as possible. Functions that operate on data structures are prefixed by their name. In these cases the first parameter to the function will be a pointer to a data structure of that type.

|Data Structure|Struct Name|Example Operation|
|---|---|---|
|Dynamic Array|cgs_array|cgs_array_length|
|Hash Table|cgs_hashtab|cgs_hashtab_lookup|
|Red-Black Tree|cgs_rbt|cgs_rbt_insert|
|Heap|cgs_heap|cgs_heap_push|

## Struct Definitions and Member Visibility

All structs that the user has access to are defined openly in their appropriate header files. Users have access to all members but are strongly encouraged to read them through the provided interface.

```C
// Definition in cgs_array.h
struct cgs_array {
	size_t length;
	/* additional members */
};

// Iterating through array in your code
for (size_t i = 0; i < cgs_array_length(arr); ++i)
	// Some operations..
```

Structures are also never `typedef`'d in the library though the user is obviously free to do as they choose in their own code. This decision was made partly due to my personal preference for snake_case and also fits with the Linux Kernel Coding Style.

The only place `typedef` is used in the library is for function signatures of utility functions.