# Installation

## Download

Either clone the repository or download the ZIP file according to your preference.

## Build

Currently the library supports building with CMake. To start, change directory to the project root directory:

```
$ cd /path/to/libcgs
```

Libcgs supports several build configurations. It is recommended that you create separate build directories for each configuration that you wish to use.

For general use, create a `Release` directory and change to it:

```
$ mkdir build-release
$ cd build-release
```

### CMake Instructions

The following instructions assume no prior knowledge with using CMake. Install CMake either using a package manager or from source.

#### Generating Build Files

Run the following command to generate the necessary build files for an optimized static library installation:

```
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```



##### Shared Library Alternative

If you prefer to build and install a shared library you can do so by replacing the above command with:

```
$ cmake -DBUILD_SHARED_LIBS=Yes -DCMAKE_BUILD_TYPE=Release ..
```

#### Building the Library

Run the following command to build the library using the default compiler for your system:

```
$ cmake --build .
```

#### Install the Library

There are two ways to go about installing your library, system-wide with a privileged install:

```
$ sudo cmake --install .
```

Or, if you do not have administrator priviledges on your system:

```
$ cmake --install . --prefix "$HOME/.local"
```

Either way, pay attention to the output and note the location that the library was installed. Here is an example from a non-privileged install:

```
[chrinkus@localhost build-static]$ cmake --install . --prefix "$HOME/.local"
-- Install configuration: "Release"
-- Installing: /home/chrinkus/.local/lib64/libcgs.a
-- Installing: /home/chrinkus/.local/include/cgs/cgs.h
(continues..)
```

Note in the above output that the header files have been installed to `/home/chrinkus/.local/include` and the static library was installed to `/home/chrinkus/.local/lib64`.

## Usage

The following short program can be used to test that the library is installed correctly on your system:

```C
#include <stdio.h>
#include <cgs/cgs.h>

int main(void)
{
	struct cgs_string s = { 0 };
	cgs_string_new_from_string(&s, "Hello World!");
	
	printf("%s\n", cgs_string_data(&s));
	
	cgs_string_free(&s);
}
```

Compile the above program and pass the locations of the headers and library with the command (your locations may be different, see the above section):

```
$ gcc -I"$HOME/.local/include" -o hello hello_world.c -L"$HOME/.local/lib64" -lcgs
```

Finally run the program:

```
$ ./hello
Hello World!
```

## Troubleshooting

If you build the project as a shared library you will need to provide the runtime path in the compile command:

```
$ gcc -I"$HOME/.local/include" -o hello hello_world.c -lcgs -Wl,-R"$HOME/.local/lib64"
```