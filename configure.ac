dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(PlotPhase, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

PKG_CHECK_MODULES([gtk], [gtk+-3.0 gtkmm-3.0])
PKG_CHECK_MODULES([gl], [gl, glu])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

