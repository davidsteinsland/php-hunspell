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

PHP_ARG_ENABLE(developer-flags, whether to enable developer build flags,
  [  --enable-developer-flags   Enable developer flags],, no)

if test "$PHP_DEVELOPER_FLAGS" = "yes"; then
  dnl Warn about functions which might be candidates for format attributes
  PHP_CHECK_GCC_ARG(-Wmissing-format-attribute,       _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wmissing-format-attribute")
  dnl Avoid duplicating values for an enum
  PHP_CHECK_GCC_ARG(-Wduplicate-enum,                 _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wduplicate-enum")
  dnl Warns on mismatches between #ifndef and #define header guards
  PHP_CHECK_GCC_ARG(-Wheader-guard,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wheader-guard")
  dnl logical not of a non-boolean expression
  PHP_CHECK_GCC_ARG(-Wlogical-not-parentheses,        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-not-parentheses")
  dnl Warn about suspicious uses of logical operators in expressions
  PHP_CHECK_GCC_ARG(-Wlogical-op,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op")
  dnl memory error detector.
  dnl FIXME: -fsanitize=address,undefined for clang. The PHP_CHECK_GCC_ARG macro isn't happy about that string :(
  PHP_CHECK_GCC_ARG(-fsanitize-address,               _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fsanitize-address")
  dnl Enable frame debugging
  PHP_CHECK_GCC_ARG(-fno-omit-frame-pointer,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-omit-frame-pointer")
  dnl Make sure we don't optimize calls
  PHP_CHECK_GCC_ARG(-fno-optimize-sibling-calls,      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-optimize-sibling-calls")
  PHP_CHECK_GCC_ARG(-Wlogical-op-parentheses,         _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op-parentheses")
  PHP_CHECK_GCC_ARG(-Wbool-conversion,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wbool-conversion")
  PHP_CHECK_GCC_ARG(-Wloop-analysis,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wloop-analysis")
  PHP_CHECK_GCC_ARG(-Wsizeof-array-argument,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wsizeof-array-argument")
  PHP_CHECK_GCC_ARG(-Wstring-conversion,              _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wstring-conversion")
  PHP_CHECK_GCC_ARG(-Wno-variadic-macros,             _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-variadic-macros")
  PHP_CHECK_GCC_ARG(-Wno-sign-compare,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-sign-compare")
  PHP_CHECK_GCC_ARG(-fstack-protector,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fstack-protector")
  PHP_CHECK_GCC_ARG(-fno-exceptions,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-exceptions")
  PHP_CHECK_GCC_ARG(-Wformat-security,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-security")
  PHP_CHECK_GCC_ARG(-Wformat-nonliteral,              _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-nonliteral")
  PHP_CHECK_GCC_ARG(-Winit-self,                      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Winit-self")
  PHP_CHECK_GCC_ARG(-Wwrite-strings,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wwrite-strings")
  PHP_CHECK_GCC_ARG(-Wenum-compare,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wenum-compare")
  PHP_CHECK_GCC_ARG(-Wempty-body,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wempty-body")
  PHP_CHECK_GCC_ARG(-Wparentheses,                    _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wparentheses")
  PHP_CHECK_GCC_ARG(-Wdeclaration-after-statement,    _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wdeclaration-after-statement")

  if test "$PHP_FOUND_VERNUM" -ge "50400"; then
    PHP_CHECK_GCC_ARG(-Werror,                        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Werror")
  fi

  MAINTAINER_CFLAGS="-Wextra $_MAINTAINER_CFLAGS  -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-missing-field-initializers"
  STD_CFLAGS="-g -O0 -Wall"
  dnl EXTRA_LDFLAGS="-Wl,--no-undefined"
fi
