dnl aclocal macro file for id3lib
dnl -- Juliano Ravasi Ferraz <jferraz at users sourceforge net>

dnl ID3LIB_CHECK(MIN-VERSION [, ACTION_IF_FOUND [, ACTION-IF-NOT-FOUND]])
dnl checks for installed id3lib and sets ID3LIB_CFLAGS and ID3LIB_LIBS
AC_DEFUN([ID3LIB_CHECK], [
  succeeded=yes

  AC_SEARCH_LIBS([ID3Tag_Link],
                 ["id3" "id3 -lstdc++" "id3 -lz" "id3 -lstdc++ -lz"],,
                 [succeeded=no])

  if test $succeeded = yes; then
    AC_MSG_CHECKING([whether id3 version >= $1])
    AC_RUN_IFELSE([
      AC_LANG_PROGRAM([[
#include <id3.h>
#include <stdio.h>
      ]], [[
  int req_maj = 0, req_min = 0, req_rev = 0;
  sscanf("$1", "%d.%d.%d", &req_maj, &req_min, &req_rev);
  return ((ID3LIB_MAJOR_VERSION < req_maj) ||
          (ID3LIB_MAJOR_VERSION == req_maj && ID3LIB_MINOR_VERSION < req_min) ||
          (ID3LIB_MAJOR_VERSION == req_maj && ID3LIB_MINOR_VERSION == req_min && ID3LIB_PATCH_VERSION < req_rev));
      ]])
    ], [
        succeeded=yes
    ], [
        succeeded=no
    ])

    if test $succeeded = yes; then
      AC_MSG_RESULT([yes])
      ifelse([$2], , :, [$2])
    else
      AC_MSG_RESULT([no])
      ifelse([$3], , AC_MSG_ERROR([id3lib version not met requirements.]), [$3])
    fi
  else
    AC_MSG_RESULT([no])
    ifelse([$3], , AC_MSG_ERROR([id3lib not found.]), [$3])
  fi
])

dnl vim: set sw=2 et:
