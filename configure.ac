#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([wether], [0.1.0], [jiangyonghan@hotmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(foreign -Wall)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([locale.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([create_main_window])

PKG_CHECK_MODULES(GTK, [gtk+-3.0 >= 3.0.0])

PACKAGE_CFLAGS="-g -Wall $GTK_CFLAGS"
PACKAGE_LIBS="-g $GTK_LIBS"
PACKAGE_LDFLAGS="-export-dynamic $GTK_LDFLAGS"

AC_SUBST([PACKAGE_CFLAGS])
AC_SUBST([PACKAGE_LIBS])
AC_SUBST([PACKAGE_LDFLAGS])

AM_INIT_AUTOMAKE

AC_CONFIG_FILES([Makefile
		 doc/Makefile
                 pic/Makefile
                 src/Makefile
                 src/ui/Makefile])
AC_OUTPUT
