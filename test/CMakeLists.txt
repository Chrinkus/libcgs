include_directories("${PROJECT_SOURCE_DIR}/include")

set(test_sources
	"tests_bst.c"
	"tests_variant.c"
	"tests_variant_usage.c"
	"tests_string_utils.c"
)

string(LENGTH "tests_" test_prefix_len)

foreach(file IN LISTS test_sources)
	get_filename_component(file_we "${file}" NAME_WE)
	string(SUBSTRING "${file_we}" ${test_prefix_len} -1 func)
	set(command_name "${func}_test")

	add_executable("${command_name}" "${file}")
	target_link_libraries("${command_name}" PRIVATE "${LIB_NAME}")
	add_test(NAME "${func}" COMMAND "${command_name}")
endforeach()

