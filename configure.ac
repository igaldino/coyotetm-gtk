dnl Process this file with autoconf to produce a configure script.

AC_INIT([coyotetm], [1.0],[igaldino@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AM_PROG_CC_C_O

PKG_CHECK_MODULES([COYOTETM], [gobject-2.0] [gom-1.0] [gtk+-3.0])

AC_CONFIG_FILES([Makefile src/Makefile])

AM_MAINTAINER_MODE

AC_OUTPUT

