#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)

# Version information.
define(JMLIST_MAJOR_VERSION, 0)
define(JMLIST_MINOR_VERSION, 0)
define(JMLIST_MICRO_VERSION, 1)

# Autoconf init.
AC_INIT([jMList], 
	[JMLIST_MAJOR_VERSION.JMLIST_MINOR_VERSION.JMLIST_MICRO_VERSION],
	[jferraz at users sourceforge net], [jmlist])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([admin])
AC_CONFIG_LIBOBJ_DIR([lib])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Versioning.
AC_DEFINE(PACKAGE_VERSION_MAJOR, JMLIST_MAJOR_VERSION,
    [Define to the major version of this package.])
AC_DEFINE(PACKAGE_VERSION_MINOR, JMLIST_MINOR_VERSION,
    [Define to the minor version of this package.])
AC_DEFINE(PACKAGE_VERSION_MICRO, JMLIST_MICRO_VERSION,
    [Define to the micro version of this package.])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([popt], [poptGetContext])
pkg_modules="glib-2.0 >= 2.4.0 libxml-2.0 >= 2.6.0"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])


# Id3lib libraries (taken from easytag-1.99.3)
# Check which libs are required by id3lib, libid3.la is fucked up
AC_SEARCH_LIBS(ID3Tag_Link, "id3" "id3 -lstdc++" "id3 -lz" "id3 -lz -lstdc++",, AC_MSG_ERROR([id3lib not found]))
# Needed to link correctly with id3lib 3.8.0
LIBS="-lz -lstdc++ $LIBS"

# expected version for cross compiling
ID3LIB_MAJOR=3
ID3LIB_MINOR=8
ID3LIB_PATCH=0

AC_MSG_CHECKING(for id3lib version)
      AC_TRY_RUN([
#include <id3.h>
#include <stdio.h>
int 
main ()
{
    FILE *output;
    output=fopen("conftest.id3","w");
    fprintf(output,"ID3LIB_MAJOR=%d\nID3LIB_MINOR=%d\nID3LIB_PATCH=%d\n",ID3LIB_MAJOR_VERSION,ID3LIB_MINOR_VERSION,ID3LIB_PATCH_VERSION);
    fclose(output);
    exit(0);
}
], . ./conftest.id3; echo "${ID3LIB_MAJOR}.${ID3LIB_MINOR}.${ID3LIB_PATCH}", AC_MSG_WARN([could not determine id3lib version]),[echo $ac_n "cross compiling; assuming ${ID3LIB_MAJOR}.${ID3LIB_MINOR}.${ID3LIB_PATCH} $ac_c"])
AC_DEFINE_UNQUOTED(ID3LIB_MAJOR, $ID3LIB_MAJOR, [id3lib major version])
AC_DEFINE_UNQUOTED(ID3LIB_MINOR, $ID3LIB_MINOR, [id3lib minor version])
AC_DEFINE_UNQUOTED(ID3LIB_PATCH, $ID3LIB_PATCH, [id3lib patch level])


AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

# GCC defaults
CFLAGS="$CFLAGS -ansi -Wall -Wno-format-y2k"

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([localtime_r memset])

# Output.
AC_CONFIG_FILES([
	Makefile
	doc/Makefile
	src/Makefile
	lib/Makefile
	styles/Makefile
])

AC_OUTPUT
