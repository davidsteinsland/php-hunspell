dnl Hunspell PHP Extension
PHP_ARG_ENABLE(hunspell, whether to enable hunspell extension support,
[  --enable-hunspell       Enable hunspell extension support])

if test $PHP_HUNSPELL != "no"; then
  LIBNAME=hunspell
  LIBSYMBOL=Hunspell_create

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  AC_MSG_CHECKING(for libhunspell)

  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists hunspell; then
    HUNSPELL_INC=`$PKG_CONFIG --cflags hunspell`
    HUNSPELL_LIB=`$PKG_CONFIG --libs hunspell`
    HUNSPELL_VER=`$PKG_CONFIG hunspell --modversion`

    AC_MSG_RESULT(version $HUNSPELL_VER found)
  else
    AC_MSG_ERROR(pkgconfig and libhunspell must be installed)
  fi

  PHP_EVAL_INCLINE($HUNSPELL_INC)
  PHP_EVAL_LIBLINE($HUNSPELL_LIB, HUNSPELL_SHARED_LIBADD)
  AC_DEFINE(HAVE_LIBHUNSPELL, 1, [Have libhunspell])

  PHP_SUBST(HUNSPELL_SHARED_LIBADD)
  PHP_NEW_EXTENSION(hunspell, php_hunspell.c, $ext_shared)
fi
