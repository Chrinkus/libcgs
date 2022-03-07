# libcgs

A C library of standard utilities.

|Type|Description|
|----|-----------|
|cgs\_array|A generic memory managed array|
|cgs\_rbt|A red-black tree|
|cgs\_string|A memory managed string for unknown reads|
|cgs\_variant|A type-safe union|
|cgs\_io|Memory managed stream reading|
|cgs\_string\_utils|Common utilities for regular `char*`'s|

## Download

Download the repo from github.

```
$ git clone https://github.com/Chrinkus/libcgs.git
$ cd libcgs
```

## CMake

### Simple Build and Install

The following set of commands builds a generic-configured static library using the default generator (make on Linux). This also assumes you have administrator privledges on the machine.

```
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
$ sudo cmake --install .
```

### Non-Privledged Install

If you don't have administrator permissions then you can install the library to another location, preferably one within your $PATH.

```
$ cmake --install . --prefix "$HOME/.local"
```

> This was a big issue for me when I got into programming so I feel I need to address it. I was concerned about "messing up" my environment so I would never install with 'sudo'. I would create project specific folders, install stuff there and append a growing list of locations to my $PATH.
>
> The recommended method of non-admin installation is to have a `.local` folder in your $HOME directory that mirrors the structure of `/usr/local`. Then you need to add this one location to the start of your $PATH. Most build systems allow you to set a 'prefix' similar to the above CMake example.

### Advanced Build and Install

This set of commands will configure the project for a release build and use Ninja to generate the project files. This also builds the project as a shared library.

```
$ mkdir build-release
$ cd build-release
$ cmake -G Ninja -DBUILD_SHARED_LIBS=YES -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build .
$ sudo cmake --install .
```

## Debug and Testing

Tests are not built by default. To build a debug-enabled version of the library and run tests enter the following from the project root:

```
$ mkdir build-debug
$ cd build-debug
$ cmake -G Ninja -DBUILD_SHARE_LIBS=YES -DCMAKE_BUILD_TYPE=Debug ..
$ cmake --build . --target tests
$ ctest
```

## Usage

To use libcgs, just the single header is required:
```
#include <stdio.h>

#include <cgs/cgs.h>

int main(void)
{
	struct cgs_rbt* tree = cgs_rbt_new(cgs_int_cmp);

	printf("Enter as many numbers as you like:\n");

	struct cgs_variant v;
	for (int n; scanf("%d ", &n) == 1; ) {
		cgs_variant_set_int(&v, n);
		cgs_rbt_insert(tree, &v);
	}

	printf("You entered %zu numbers.\n", cgs_rbt_size(tree));

	const int* min = cgs_rbt_min(tree);
	printf("The minimum value was %d.\n", *min);
	printf("The maximum value was %d.\n", *(int*)cgs_rbt_max(tree));

	cgs_rbt_free(tree);

	return 0;
}
```

Compile and run the above example with:
```
$ gcc test.c -o cgstest -lcgs
$ ./cgstest
Enter as many numbers as you like:
14
53
-8
2
0
37
19
<Ctrl-D>
You entered 7 numbers.
The minimum value was -8.
The maximum value was 53.
```
