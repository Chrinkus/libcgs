#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([libcgs], [0.1], [https://github.com/Chrinkus/libcgs/issues])
AM_INIT_AUTOMAKE([-Wall foreign])
LT_PREREQ([2.4.6])
LT_INIT
AC_CONFIG_SRCDIR([src/cgs_bst.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
		 test/Makefile])
AC_OUTPUT
