cmake_minimum_required(VERSION 3.18)

project(libcgs VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra)

include(GNUInstallDirs)

if(APPLE)
	include_directories(${CMAKE_INSTALL_PREFIX}/include)
	link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

set(LIB_NAME "cgs")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

# Generates cmake config file for discovery
install(EXPORT ${LIB_NAME}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
	NAMESPACE ${LIB_NAME}::
	FILE ${LIB_NAME}-config.cmake
)

