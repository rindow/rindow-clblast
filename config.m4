dnl config.m4 for extension rindow_clblast

PHP_ARG_ENABLE(rindow_clblast, whether to enable rindow_clblast support,
dnl Make sure that the comment is aligned:
[  --enable-rindow_clblast          Enable rindow_clblast support], no)
PHP_ARG_WITH(rindow_opencl, rindow_opencl support,
[  --with-rindow_opencl          Specify rindow_opencl path])

if test "$PHP_RINDOW_CLBLAST" != "no"; then

  dnl # get library CLblast build options from pkg-config output
  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  AC_MSG_CHECKING(for clblast)
  if test -x "$PKG_CONFIG"; then
    if $PKG_CONFIG --exists OpenCL; then
      if $PKG_CONFIG OpenCL --atleast-version 1.2.0; then
        LIBOPENCL_CFLAGS="`$PKG_CONFIG OpenCL --cflags`"
        LIBOPENCL_LIBDIR="`$PKG_CONFIG OpenCL --libs`"
        LIBOPENCL_VERSON=`$PKG_CONFIG OpenCL --modversion`
        AC_MSG_RESULT(libOpenCL: version $LIBOPENCL_VERSON)
      else
        AC_MSG_ERROR(system OpenCL is too old: version 1.2.0 required)
      fi
    else
      AC_MSG_ERROR(OpenCL not found)
    fi
    if $PKG_CONFIG --exists clblast; then
      if $PKG_CONFIG clblast --atleast-version 1.5.1; then
        LIBCLBLAST_CFLAGS="`$PKG_CONFIG clblast --cflags`"
        LIBCLBLAST_LIBDIR="`$PKG_CONFIG clblast --libs`"
        LIBCLBLAST_VERSON=`$PKG_CONFIG clblast --modversion`
        AC_MSG_RESULT(clblast version $LIBCLBLAST_VERSON)
      else
        AC_MSG_ERROR(system clblast is too old: version 1.5.1 required)
      fi
    else
      AC_MSG_ERROR(clblast not found)
    fi
  else
    AC_MSG_ERROR(pkg-config not found)
  fi
  AC_DEFINE(CL_TARGET_OPENCL_VERSION, 120, [ Target OpenCL version 1.2 ])
  PHP_EVAL_LIBLINE($LIBOPENCL_LIBDIR" "$LIBCLBLAST_LIBDIR, RINDOW_CLBLAST_SHARED_LIBADD)
  PHP_EVAL_INCLINE($LIBOPENCL_CFLAGS" "$LIBCLBLAST_CFLAGS)

  AC_MSG_CHECKING(for Interop/Polite/Math/Matrix.h)
  if test -f "PHP_EXT_SRCDIR(rindow_clblast)/vendor/interop-phpobjects/polite-math/include/Interop/Polite/Math/Matrix.h" ; then
    AC_MSG_RESULT(ok)
    PHP_ADD_INCLUDE(PHP_EXT_SRCDIR(rindow_clblast)[/vendor/interop-phpobjects/polite-math/include])
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR(Interop/Polite/Math/Matrix.h not found. Please type "composer update")
  fi

  AC_MSG_CHECKING(for Rindow/OpenCL/Buffer.h)
  if test -f "$PHP_RINDOW_OPENCL/include/Rindow/OpenCL/Buffer.h" ; then
    AC_MSG_RESULT(ok)
    PHP_ADD_INCLUDE("$PHP_RINDOW_OPENCL/include")
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR(Rindow/OpenCL/Buffer.h not found. Please add "--with-rindow_opencl" option)
  fi

  PHP_SUBST(RINDOW_CLBLAST_SHARED_LIBADD)

  RINDOW_CLBLAST_SOURCES="\
     rindow_clblast.c \
     src/Rindow/CLBlast/Blas.c \
     src/Rindow/CLBlast/Math.c \
  "

  PHP_NEW_EXTENSION(rindow_clblast, $RINDOW_CLBLAST_SOURCES, $ext_shared)
fi
