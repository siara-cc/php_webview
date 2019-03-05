#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define WEBVIEW_IMPLEMENTATION
#include <stdio.h>
#include <limits.h>
#include "php.h"
#include "php_ini.h"
#include "php_webview.h"
#include "webview.h"

const int pass_rest_by_reference = 0;

zval *cbFnStr;
void my_cb(struct webview *w, const char *arg);
struct webview wv = {
    .title = "Webview on PHP",
    .url = "data:text/html,%3C%21doctype%20html%3E%3Chtml%3E%3Cbody%3EHello%20World%3C%2Fbody%3E%3C%2Fhtml%3E",
    .width = 800,
    .height = 600,
    .debug = 1,
    .resizable = 1,
    .external_invoke_cb = my_cb,
};

ZEND_BEGIN_ARG_INFO(NoArgByReference1, pass_rest_by_reference)
   ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(NoArgByReference2, pass_rest_by_reference)
   ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO()

static zend_function_entry webview_functions[] = {
    PHP_FE(webview, NoArgByReference1)
    PHP_FE(webview_eval, NoArgByReference2)
    PHP_FE_END
};

zend_module_entry webview_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_WEBVIEW_EXTNAME,
    webview_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_WEBVIEW_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_WEBVIEW
ZEND_GET_MODULE(webview)
#endif

PHP_FUNCTION(webview)
{
  zval *urlStr = NULL;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz",
        &urlStr, &cbFnStr) == FAILURE) {
    printf("Failed\n");
    return;
  }
  wv.url = Z_STRVAL_P(urlStr);
  int blocking = 1;
  // Create webview window using the provided options
  webview_init(&wv);
  // To change window title
  //webview_set_title(&wv, "New title");
  // Main app loop, can be either blocking or non-blocking
  while (webview_loop(&wv, blocking) == 0);
  // Destroy webview window, often exits the app
  webview_exit(&wv);
  // To terminate the webview main loop:
  webview_terminate(&wv);
  // To print logs to stderr, MacOS Console or DebugView
  //webview_debug("exited: %d\n", 1);
  //RETURN_STRING("Hello World", 1);
}

PHP_FUNCTION(webview_eval)
{

  zval *scr = NULL;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z",
        &scr) == FAILURE) {
    printf("Failed\n");
    return;
  }
  webview_eval(&wv, Z_STRVAL_P(scr));

}

void my_cb(struct webview *w, const char *arg) {
  //char msg[1000];
  //printf("Hello: %s\n", arg);
  //snprintf(msg, sizeof(msg), "alert('%s')", arg);

  zval p1;
#if PHP_MAJOR_VERSION >= 7
  ZVAL_NULL(&p1);
  ZVAL_STRING(&p1, arg);
#else
  INIT_ZVAL(&p1);
  ZVAL_STRING(&p1, arg, 1);
#endif
  zval *params = { &p1 };
  //zend_uint param_count = 1;
  zval retval;

  zval function_name;
#if PHP_MAJOR_VERSION >= 7
  ZVAL_NULL(&function_name);
  ZVAL_STRING(&function_name, Z_STRVAL_P(cbFnStr));
#else
  INIT_ZVAL(function_name);
  ZVAL_STRING(&function_name, Z_STRVAL_P(cbFnStr), 1);
#endif

#if PHP_MAJOR_VERSION >= 7
  if (call_user_function(CG(function_table), NULL, // no object
        &function_name, &retval, 1,
        params TSRMLS_CC) == SUCCESS) {
#else
  if (call_user_function(CG(function_table), NULL, // no object
        &function_name, &retval, 1,
        &params TSRMLS_CC) == SUCCESS) {
#endif
      //printf("Success returning from PHP\n");
      if (Z_TYPE_P(&retval) == IS_STRING) {
        char *cstr;
        size_t cstrlen;
        cstr = Z_STRVAL_P(&retval);
        cstrlen = Z_STRLEN_P(&retval);
        cstr[cstrlen] = 0;
        if (cstrlen)
          webview_eval(w, cstr);
      }
      zval_dtor(&retval);
  }

  // free the zvals
  zval_dtor(&function_name);
  zval_dtor(&p1);
}

