include_directories("${PROJECT_SOURCE_DIR}/include")

set(sources
	"cgs_bst.c"
	"cgs_variant.c"
)

add_library(${LIB_NAME} "${sources}")

