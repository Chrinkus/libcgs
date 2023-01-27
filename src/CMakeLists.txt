add_library(${LIB_NAME}
	"cgs_bst.c"
	"cgs_compare.c"
        "cgs_error.c"
        "cgs_hashtab.c"
        "cgs_heap.c"
	"cgs_io.c"
        "cgs_iter.c"
        "cgs_numeric.c"
	"cgs_rbt.c"
	"cgs_sort.c"
	"cgs_string.c"
	"cgs_string_utils.c"
	"cgs_variant.c"
	"cgs_vector.c"
)
target_include_directories(${LIB_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
