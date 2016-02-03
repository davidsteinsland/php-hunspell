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
}

PHP_METHOD(Hunspell, checkSpelling) {
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

  if (Hunspell_spell(hunspell->hspell, word) == 0) {
    RETURN_FALSE;
  }

  RETURN_TRUE;
}

PHP_METHOD(Hunspell, suggestSpelling) {
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
  int ns = Hunspell_suggest(hunspell->hspell, &suggestions, word);

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
  PHP_ME(Hunspell, checkSpelling,   NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Hunspell, suggestSpelling, NULL, ZEND_ACC_PUBLIC)
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
