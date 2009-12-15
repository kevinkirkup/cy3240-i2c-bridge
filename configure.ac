#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT(cy3240_i2c, 0.1.0, kevin.kirkup@sonyericsson.com)
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Initialize automake
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strtol])

# Custom Args
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug], [Enable debug build support]),
              [build_debug=$enableval],
              [build_debug=no])

if test $build_debug = yes; then
     AC_DEFINE(DEBUG, 1, [1 to enable debug])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
