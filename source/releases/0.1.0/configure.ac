#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)


### Version information.
define(JMLIST_MAJOR_VERSION, 0)
define(JMLIST_MINOR_VERSION, 1)
define(JMLIST_MICRO_VERSION, 0)
define(JMLIST_URL, "http://jmlist.berlios.de/")


### Autoconf init.
AC_INIT([jMList], 
	[JMLIST_MAJOR_VERSION.JMLIST_MINOR_VERSION.JMLIST_MICRO_VERSION],
	[jferraz at users sourceforge net], [jmlist])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([admin])
AC_CONFIG_LIBOBJ_DIR([lib])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE


### URL and Versioning.
AC_DEFINE(PACKAGE_URL, JMLIST_URL,
    [Define to the website address of this package.])
AC_DEFINE(PACKAGE_VERSION_MAJOR, JMLIST_MAJOR_VERSION,
    [Define to the major version of this package.])
AC_DEFINE(PACKAGE_VERSION_MINOR, JMLIST_MINOR_VERSION,
    [Define to the minor version of this package.])
AC_DEFINE(PACKAGE_VERSION_MICRO, JMLIST_MICRO_VERSION,
    [Define to the micro version of this package.])


### Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_PATH_PROG([XSLTPROC], [xsltproc])
if test "x$XSLTPROC" == "x"; then
    AC_MSG_ERROR([xsltproc (part of libxslt) not found on \$PATH.])
fi


### Checks for libraries.

# popt library (ftp://ftp.rpm.org/pub/rpm/dist/rpm-4.1.x/)
# We have a replacement for this dependency; so, if it isn't found on the
# system, we just fall to using our own.
AC_CHECK_LIB([popt], [poptGetContext])
AM_CONDITIONAL([HAVE_LIBPOPT], [test _$ac_cv_lib_popt_poptGetContext = _yes])

# glib (http://www.gtk.org/)
# libxml (http://xmlsoft.org/)
# libxslt (http://xmlsoft.org/XSLT/)
pkg_modules="glib-2.0 >= 2.4.0 libxml-2.0 >= 2.6.0 libxslt >= 1.1.0"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])

# id3lib (http://id3tag.sourceforge.net/)
ID3LIB_CHECK([3.8.0])

# libogg (http://www.xiph.org/)
# libvorbis (http://www.xiph.org/)
ogg_support=no
vorbis_support=no

XIPH_PATH_OGG([
    ogg_support=yes
    AC_DEFINE([HAVE_LIBOGG], [1],
	[Define to 1 if you have the ogg library (-logg).])
])

if test _$ogg_support = _yes; then
    XIPH_PATH_VORBIS([
	vorbis_support=yes
	AC_DEFINE([HAVE_LIBVORBIS], [1],
	    [Define to 1 if you have the vorbis library (-lvorbis).])
    ])
fi

if test _$ogg_support != _yes; then AC_MSG_WARN([OGG support disabled.]); fi
if test _$vorbis_support != _yes; then AC_MSG_WARN([OGG Vorbis support disabled.]); fi

AM_CONDITIONAL([HAVE_LIBOGG], [test _$ogg_support = _yes])
AM_CONDITIONAL([HAVE_LIBVORBIS], [test _$vorbis_support = _yes])


### GCC defaults
CFLAGS="$CFLAGS -Wall"


### Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([alloca.h fcntl.h float.h libintl.h limits.h locale.h mcheck.h wchar.h])


### Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_OFF_T
AC_STRUCT_TM


### Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([geteuid getuid localtime_r memset mtrace setlocale setregid strchr strerror strrchr strtol __secure_getenv])


### Directory paths.
CPPFLAGS="$CPPFLAGS -DDATADIR='\"$datadir\"'"


### Output.
AC_CONFIG_FILES([
	Makefile
	doc/Makefile
	src/Makefile
	lib/Makefile
	lib/popt/Makefile
	styles/Makefile
])

AC_OUTPUT
