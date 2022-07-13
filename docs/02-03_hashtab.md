# Hash Table

A hash table implementation of string keys and generic values.

```C
#include <cgs/cgs_hashtab.h>

typedef size_t (*CgsHashFunc)(const void* key, size_t size);

struct cgs_hashtab {
	size_t length;				// Number of elements
	size_t size;				// Size of hash table
	struct cgs_bucket** table;
	double max_load;			// Ratio of length to size
	CgsHashFunc hash;
	CgsCmp3Way cmp;
};
```

The hash table also relies on some external tools from `libcgs`:

```C
#include <cgs/cgs_defs.h>

typedef int (*CgsCmp3Way)(const void* a, const void* b);

#include <cgs/cgs_variant.h>

struct cgs_variant;		// A generic type
```

## Hash Table API

|Function|What it does|
|---|---|
|`cgs_hashtab_new`|Allocates and initializes a new hash table.|
|`cgs_hashtab_free`|Frees the memory of a hash table.|
|`cgs_hashtab_length`|Get the number of elements in the hash table.|
|`cgs_hashtab_lookup`|Check table for a given key. If found returns a read-only pointer to the value. If not found returns NULL.|
|`cgs_hashtab_get`|Check the table for a given key. If found returns a pointer to the value's containing `cgs_variant`. If not found adds a new bucket to the table and returns a pointer to the value's containing `cgs_variant`.|
|`cgs_hashtab_remove`|Removes a bucket with the matching key if found.|

## Memory Management