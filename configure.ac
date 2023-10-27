#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nikita-gtk], [0.1.0], [ole@aamot.org])
AM_INIT_AUTOMAKE([-Wno-portability 1.11 no-dist-gzip dist-xz tar-ustar subdir-objects])
AC_CONFIG_SRCDIR([nikita-gtk.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Checks for libraries.

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(ARKIVARIUM,
		  glib-2.0 >= 2.38.0 \
  		  gtk+-3.0 >= 3.22.30 \
		  libsoup-2.4 >= 2.74.3 \
		  libxml-2.0 >= 2.0)

ARKIVARIUM_PKG_DEPS="glib-2.0 gobject-2.0 gtk+-3.0 libxml-2.0 libsoup-2.4"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT(
Makefile
)
