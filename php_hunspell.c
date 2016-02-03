#include "php_hunspell.h"
#include <hunspell/hunspell.h>

static zend_class_entry* hunspell_ce;

/*
  excellent reading material: http://www.phpinternalsbook.com/classes_objects/custom_object_storage.html
 */
static zend_object_handlers hunspell_object_handlers;

typedef struct _hunspell_object {
  zend_object std;
  Hunhandle* hspell;
} hunspell_object;

static void hunspell_free_storage(hunspell_object* hspell TSRMLS_DC) {
  Hunspell_destroy(hspell->hspell);
  zend_object_std_dtor(&hspell->std TSRMLS_CC);
  efree(hspell);
}

zend_object_value hunspell_create_handler(zend_class_entry* class_type TSRMLS_DC) {
   zend_object_value retval;

  /* Allocate and zero-out the internal object structure. By convention the variable holding
   * the internal structure is usually called `intern`. */
  hunspell_object* intern = (hunspell_object*) emalloc(sizeof(hunspell_object));
  memset(intern, 0, sizeof(hunspell_object));

  /* The underlying std zend_object has to be initialized.  */
  zend_object_std_init(&intern->std, class_type TSRMLS_CC);

  /* Even if you don't use properties yourself you should still call object_properties_init(),
   * because extending classes may use properties. (Generally a lot of the stuff you will do is
   * for the sake of not breaking extending classes). */
  object_properties_init(&intern->std, class_type);

  /* Put the internal object into the object store, with the default dtor handler and our custom
   * free handler. The last NULL parameter is the clone handler, which is left empty for now. */
  retval.handle = zend_objects_store_put(
    intern,
    (zend_objects_store_dtor_t) zend_objects_destroy_object,
    (zend_objects_free_object_storage_t) hunspell_free_storage,
    NULL TSRMLS_CC
  );

  /* Assign the customized object handlers */
  retval.handlers = &hunspell_object_handlers;

  return retval;
}

PHP_METHOD(Hunspell, __construct) {
  Hunhandle* hunspell = NULL;
  zval* object = getThis();

  char* affix;
  char* dict;
  int affix_len;
  int dict_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "pp", &affix, &affix_len, &dict, &dict_len) != SUCCESS) {
    return;
  }

  hunspell = Hunspell_create(affix, dict);
  hunspell_object* obj = (hunspell_object*)zend_object_store_get_object(object TSRMLS_CC);
  obj->hspell = hunspell;
}

PHP_METHOD(Hunspell, addDictionary) {
#if HAVE_LIBHUNSPELL_ADD_DIC
  hunspell_object* hunspell;

  char* dpath;
  int dpath_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p", &dpath, &dpath_len) != SUCCESS) {
    return;
  }

  hunspell = (hunspell_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

  if (Hunspell_add_dic(hunspell->hspell, dpath)) {
    RETURN_FALSE;
  }

  RETURN_TRUE;
#else
  RETURN_FALSE;
#endif
}


static void _hunspell_get_result(INTERNAL_FUNCTION_PARAMETERS, int mode) {
  hunspell_object* hunspell;

  char* word;
  int word_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) != SUCCESS) {
    RETURN_FALSE;
  }

  if (word_len == 0) {
    RETURN_FALSE;
  }

  hunspell = (hunspell_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

  int res;

  switch (mode) {
    case 0:
      res = Hunspell_add(hunspell->hspell, word) != 0;
      break;
    case 1:
      res = Hunspell_spell(hunspell->hspell, word) == 0;
      break;
    default:
      RETURN_FALSE;
  }

  if (res) {
    RETURN_FALSE;
  }

  RETURN_TRUE;
}

PHP_METHOD(Hunspell, add) {
  _hunspell_get_result(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

PHP_METHOD(Hunspell, spell) {
  _hunspell_get_result(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

static void _hunspell_get_results(INTERNAL_FUNCTION_PARAMETERS, int mode) {
  hunspell_object* hunspell;

  char* word;
  int word_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) != SUCCESS) {
    RETURN_FALSE;
  }

  if (word_len == 0) {
    RETURN_FALSE;
  }

  hunspell = (hunspell_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

  char** suggestions;
  int ns;

  switch (mode) {
    case 0:
      ns = Hunspell_suggest(hunspell->hspell, &suggestions, word);
      break;
    case 1:
      ns = Hunspell_analyze(hunspell->hspell, &suggestions, word);
      break;
    case 2:
      ns = Hunspell_stem(hunspell->hspell, &suggestions, word);
      break;
    default:
      RETURN_FALSE;
      break;
  }

  array_init_size(return_value, ns);

  int i;
  for (i = 0; i < ns; ++i) {
    add_next_index_string(return_value, suggestions[i], 1);
  }
  Hunspell_free_list(hunspell->hspell, &suggestions, ns);
}

PHP_METHOD(Hunspell, suggest) {
  _hunspell_get_results(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

PHP_METHOD(Hunspell, analyze) {
  _hunspell_get_results(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(Hunspell, stem) {
  _hunspell_get_results(INTERNAL_FUNCTION_PARAM_PASSTHRU, 2);
}

PHP_METHOD(Hunspell, generate) {
  hunspell_object* hunspell;

  char* word1;
  int word1_len = 0;
  char* word2;
  int word2_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &word1, &word1_len, &word2, &word2_len) != SUCCESS) {
    RETURN_FALSE;
  }

  if (word1_len == 0 || word2_len == 0) {
    RETURN_FALSE;
  }

  hunspell = (hunspell_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

  char** suggestions;
  int ns = Hunspell_generate(hunspell->hspell, &suggestions, word1, word2);

  array_init_size(return_value, ns);

  int i;
  for (i = 0; i < ns; ++i) {
    add_next_index_string(return_value, suggestions[i], 1);
  }
  Hunspell_free_list(hunspell->hspell, &suggestions, ns);
}

static const zend_function_entry hunspell_methods[] = {
  PHP_ME(Hunspell, __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  PHP_ME(Hunspell, addDictionary,   NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, add,             NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, spell,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, suggest,         NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, analyze,         NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, stem,            NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, generate,        NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

PHP_MINIT_FUNCTION(hunspell) {
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "Hunspell", hunspell_methods);
  hunspell_ce = zend_register_internal_class(&ce TSRMLS_CC);
  hunspell_ce->create_object = hunspell_create_handler;

  memcpy(&hunspell_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  hunspell_object_handlers.clone_obj = NULL;

  return SUCCESS;
}

PHP_MINFO_FUNCTION(hunspell) {
  php_info_print_table_start();
  php_info_print_table_header(2, HUNSPELL_EXTNAME, HUNSPELL_EXTVER);
  php_info_print_table_end();
}

zend_module_entry hunspell_module_entry = {
  STANDARD_MODULE_HEADER,
  HUNSPELL_EXTNAME,
  NULL,
  PHP_MINIT(hunspell), /* module initialization */
  NULL, /* module shutdown process */
  NULL, /* request initialization */
  NULL, /* reqeust shutdown process */
  PHP_MINFO(hunspell),
  HUNSPELL_EXTVER,
  STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_HUNSPELL
ZEND_GET_MODULE(hunspell)
#endif
