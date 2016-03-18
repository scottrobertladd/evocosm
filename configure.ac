AC_INIT([evocosm], [6.0.0])
AC_CONFIG_SRCDIR([evocosm/evocosm.h])
AM_INIT_AUTOMAKE([no-define dist-bzip2 dist-zip])

AC_CONFIG_MACRO_DIR([m4])

GENERIC_LIBRARY_NAME=libevocosm

#release versioning
GENERIC_MAJOR_VERSION=6
GENERIC_MINOR_VERSION=0
GENERIC_MICRO_VERSION=0

GENERIC_LIBRARY_VERSION=6:0:0

AC_SUBST(GENERIC_LIBRARY_VERSION)

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_HEADERS(stdint.h unistd.h fcntl.h)

AC_ARG_ENABLE([docgen],
              AS_HELP_STRING([--enable-docgen], [generate documentation with Doxygen]),
              [docgen=$enableval],
              [docgen=no])

if test "x$docgen" = "xyes"
then
    AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
        if test $HAVE_DOXYGEN = "false"; then
            AC_MSG_WARN([*** doxygen not found, code documentation will not be built])
        fi
    AM_CONDITIONAL(HAVE_DOXYGEN, $HAVE_DOXYGEN)
else
    AM_CONDITIONAL(HAVE_DOXYGEN, "false")
fi

AC_OUTPUT(Makefile evocosm.pc evocosm/Makefile examples/fopt/Makefile examples/pdsm/Makefile)
