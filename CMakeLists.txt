cmake_minimum_required(VERSION 3.18)

project(libcgs VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra -Wpedantic)

set(LIB_NAME "cgs")

add_subdirectory(src)
add_subdirectory(include)

enable_testing()
add_subdirectory(test)

include(GNUInstallDirs)
install(TARGETS ${LIB_NAME})

