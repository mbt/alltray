# check for LD_PRELOAD functionality
# written original by Tim Potter
# found here: 
# http://lists.samba.org/archive/samba-technical/1998-November/thread.html#2646

AC_DEFUN([AM_LD_PRELOAD],
[

AC_MSG_CHECKING([for working function preloading (LD_PRELOAD)])
export LD_PRELOAD=""

cat > ld_preload.c << EOF
#include <stdio.h>
int main (int argc, char *argv[])
{
  exit(preload_test());
}
EOF

cat > ld_preload_lib1.c << EOF
int preload_test(void)
{
  return 1;
}
EOF

cat > ld_preload_lib2.c << EOF
int preload_test(void)
{
  return 0;
}
EOF

_HAVE_LD_PRELOAD=no

if AC_TRY_COMMAND($CC -c ./ld_preload.c); then
 if AC_TRY_COMMAND($CC -shared -o ./ld_preload_lib1.so ./ld_preload_lib1.c); then
  if AC_TRY_COMMAND($CC -shared -o ./ld_preload_lib2.so ./ld_preload_lib2.c); then
   if AC_TRY_COMMAND($CC $CFLAGS $CPPFLAGS -o conftest ./ld_preload.o ./ld_preload_lib1.so); then
    LD_PRELOAD=./ld_preload_lib2.so; export LD_PRELOAD
    if ./conftest; then
     _HAVE_LD_PRELOAD=yes
    fi
   fi
  fi
 fi
fi

export LD_PRELOAD=""
rm -f conftest ld_preload.o ld_preload_lib1.so
rm -f ld_preload_lib2.so ld_preload.c ld_preload_lib1.c
rm -f ld_preload_lib2.c

AC_MSG_RESULT([$_HAVE_LD_PRELOAD])

LIBDIR=""
if test x"$_HAVE_LD_PRELOAD" = x"yes"; then
  AC_DEFINE(HAVE_LD_PRELOAD, 1, [HAVE LD_PRELOAD])
  LIBDIR="lib"
  AC_SUBST(LIBDIR)
fi

])



# Check for binary relocation support
# Hongli Lai
# http://autopackage.org/

AC_DEFUN([AM_BINRELOC],
[

  AC_CHECK_FILE([/proc/self/maps])
  AC_CACHE_CHECK([whether everything is installed to the same prefix],
    [br_cv_valid_prefixes], [
    if test "$bindir" = '${exec_prefix}/bin' -a "$sbindir" = '${exec_prefix}/sbin' -a \
    "$datadir" = '${prefix}/share' -a "$libdir" = '${exec_prefix}/lib' -a \
    "$libexecdir" = '${exec_prefix}/libexec' -a "$sysconfdir" = '${prefix}/etc'
    then
    br_cv_valid_prefixes=yes
    else
    br_cv_valid_prefixes=no
    fi
  ])
  
  AC_CACHE_CHECK([whether binary relocation is working],
    [br_cv_binreloc],
    [if test "x$br_cv_valid_prefixes" = "xyes" -a \
    "x$ac_cv_file__proc_self_maps" = "xyes"; then
    br_cv_binreloc=yes
    else
    br_cv_binreloc=no
    fi
  ])
  
  if test "x$br_cv_binreloc" = "xyes"; then
    AC_DEFINE(ENABLE_BINRELOC,1,[binary relocation working ?])
  fi
  
  AC_DEFINE(BR_PTHREAD,0,[Include pthread support for binary relocation?])

])
