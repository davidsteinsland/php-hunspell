#ifndef _PHP_HUNSPELL_H
#define _PHP_HUNSPELL_H

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#ifdef ZTS
  #include <TSRM.h>
#endif

#include "php.h"
#include "ext/standard/info.h"

#define HUNSPELL_EXTNAME "hunspell"
#define HUNSPELL_EXTVER "0.1"

extern zend_module_entry hunspell_module_entry;

PHP_MINIT_FUNCTION(hunspell);
PHP_MINFO_FUNCTION(hunspell);

#endif
