AC_PREREQ([2.69])
AC_INIT([syntheffect], [0.1], [chao-mu@hackpoetic.com])
AC_CONFIG_SRCDIR([include/syntheffect/app.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Specify C++11
AX_CXX_COMPILE_STDCXX_11

# Checks for libraries.
PKG_CHECK_MODULES(RTMIDI, rtmidi >= 2.0.1)
AM_PATH_SDL2(2.0.2)
PKG_CHECK_MODULES(OPENCV, opencv >= 3.4.1)
AX_BOOST_BASE

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
