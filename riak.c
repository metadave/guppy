/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_riak.h"
#include <riak.h>

/* If you declare any globals in php_riak.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(riak)
*/

/* True global resources - no need for thread safety here */
static int le_riak;

/* {{{ riak_functions[]
 *
 * Every user visible function must have an entry in riak_functions[].
 */
const zend_function_entry riak_functions[] = {
	PHP_FE(riak_connect,	NULL)
	PHP_FE_END	/* Must be the last line in riak_functions[] */
};
/* }}} */

/* {{{ riak_module_entry
 */
zend_module_entry riak_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"riak",
	riak_functions,
	PHP_MINIT(riak),
	PHP_MSHUTDOWN(riak),
	PHP_RINIT(riak),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(riak),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(riak),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_RIAK_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RIAK
ZEND_GET_MODULE(riak)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("riak.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_riak_globals, riak_globals)
    STD_PHP_INI_ENTRY("riak.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_riak_globals, riak_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_riak_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_riak_init_globals(zend_riak_globals *riak_globals)
{
	riak_globals->global_value = 0;
	riak_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(riak)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(riak)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(riak)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(riak)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(riak)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "riak support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto string riak_connect(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(riak_connect)
{
	char *hostname = NULL;
	int   hostlen  = 0;
	char *port     = NULL;
	int   portlen  = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &hostname, &hostlen, &port, &portlen) == FAILURE) {
		return;
	}

	printf("Connecting to Riak\n");
    riak_config *cfg;

    // use the default configuration
    riak_error err = riak_config_new_default(&cfg);
    if(err != ERIAK_OK) {
      fprintf(stderr, "Error creating client configuration\n");
      exit(1);
    }

 riak_connection *cxn = NULL;

    err = riak_connection_new(cfg, &cxn, hostname, port, NULL);

    if (err) {
        fprintf(stderr, "Cannot connect to Riak on %s:%s\n", hostname, port);
        exit(1);
    }
	printf("Connected to Riak\n");

	RETURN_TRUE;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
