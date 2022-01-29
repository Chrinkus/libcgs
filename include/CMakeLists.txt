set(headers
	"include/cgs_bst.h"
	"include/cgs_variant.h"
)

set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${headers}")

