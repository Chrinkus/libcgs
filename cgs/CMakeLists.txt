set(sources
	"cgs_bst.c"
	"cgs_string_utils.c"
	"cgs_variant.c"
)

add_library(${LIB_NAME} "${sources}")

set(headers
	"cgs.h"
	"cgs_bst.h"
	"cgs_string_utils.h"
	"cgs_test.h"
	"cgs_variant.h"
)

set_target_properties(${LIB_NAME} PROPERTIES
	VERSION 	${PROJECT_VERSION}
	SOVERSION	${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER 	"${headers}"
)

install(TARGETS ${LIB_NAME}
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}"
)

