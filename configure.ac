AC_PREREQ([2.69])
AC_INIT([secret-importer], [1.0], [nihilian@live.com])
AC_CONFIG_SRCDIR([src/secret_schema_web.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG
AC_CHECK_HEADERS([stdlib.h string.h])
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strerror])
PKG_CHECK_MODULES(LIBSECRET, [libsecret-1 >= 0.18])
AC_SUBST(LIBSECRET_CFLAGS)
AC_SUBST(LIBSECRET_LIBS)
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
