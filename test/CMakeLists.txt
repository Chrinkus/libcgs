# Get cmocka dependency
include(FetchContent)
FetchContent_Declare(cmocka
        GIT_REPOSITORY  https://gitlab.com/Chrinkus/cmocka.git
        GIT_TAG         origin/cmake
)
FetchContent_MakeAvailable(cmocka)

# Needed for private headers
include_directories("${PROJECT_SOURCE_DIR}/src")

# Test build target
add_custom_target(tests_cgs)

# List of tests
set(test_sources
	"tests_bst.c"
	"tests_compare.c"
	"tests_defs.c"
        "tests_error.c"
        "tests_hashtab.c"
        "tests_heap.c"
        "tests_heap_private.c"
	"tests_io.c"
        "tests_numeric.c"
	"tests_rbt.c"
        "tests_rbt_private.c"
	"tests_sort.c"
	"tests_variant.c"
	"tests_string.c"
	"tests_string_utils.c"
        "tests_strsub.c"
        "tests_str_split.c"
	"tests_vector.c"
        "tests_vector_string.c"
)

# For stripping prefix.
string(LENGTH "tests_" test_prefix_len)

# Each test needs 3 names:
#	file:	tests_array.c
#	test:	array
#	exe:	array_test

foreach(file IN LISTS test_sources)
	get_filename_component(file_we "${file}" NAME_WE)
	string(SUBSTRING "${file_we}" ${test_prefix_len} -1 func)
	set(command_name "${func}_test")

	add_executable("${command_name}" EXCLUDE_FROM_ALL "${file}")
	target_link_libraries("${command_name}" PRIVATE "${LIB_NAME}" cmocka)
	add_test(NAME "${func}" COMMAND "${command_name}")
	add_dependencies(tests_cgs "${command_name}")
endforeach()

# Move data files to test binary
file(COPY "${PROJECT_SOURCE_DIR}/data/io_test_data.txt"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
