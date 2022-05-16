set(sources
	"cgs_array.c"
	"cgs_bst.c"
	"cgs_compare.c"
	"cgs_io.c"
	"cgs_rbt.c"
	"cgs_sort.c"
	"cgs_string.c"
	"cgs_string_utils.c"
	"cgs_variant.c"
)

set(headers
	"cgs.h"
	"cgs_array.h"
	"cgs_bst.h"
	"cgs_compare.h"
	"cgs_defs.h"
	"cgs_io.h"
	"cgs_rbt.h"
	"cgs_rbt_private.h"
	"cgs_sort.h"
	"cgs_string.h"
	"cgs_string_utils.h"
	"cgs_test.h"
	"cgs_variant.h"
)

add_library(${LIB_NAME} "${sources}")

set_target_properties(${LIB_NAME} PROPERTIES
	VERSION 	${PROJECT_VERSION}
	SOVERSION	${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER 	"${headers}"
)

install(TARGETS ${LIB_NAME}
	EXPORT ${LIB_NAME}
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}"
)

