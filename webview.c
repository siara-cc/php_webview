#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define WEBVIEW_IMPLEMENTATION
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "php.h"
#include "php_webview.h"
#define WEBVIEW_COCOA 1
#include "webview.h"

const int pass_rest_by_reference = 0;
const int pass_arg_by_reference = 0;

char *cbFnStr = NULL;
void my_cb(struct webview *w, const char *arg);
struct webview wv = {
    .title = "Title",
    .url = "data:text/html,%3C%21doctype%20html%3E%3Chtml%3E%3Cbody%3EHello%20World%3C%2Fbody%3E%3C%2Fhtml%3E",
    .width = 800,
    .height = 600,
    .debug = 1,
    .resizable = 1,
    .external_invoke_cb = my_cb,
};

ZEND_BEGIN_ARG_INFO(NoArgByReference, pass_rest_by_reference)
   ZEND_ARG_PASS_INFO(pass_arg_by_reference)
ZEND_END_ARG_INFO()

static zend_function_entry webview_functions[] = {
    PHP_FE(webview, NoArgByReference)
    PHP_FE(webview_eval, NoArgByReference)
    {NULL, NULL, NULL}
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
  char *urlStr = NULL;
  int lenURL;
  int lenCBFn;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
        &urlStr, &lenURL, &cbFnStr, &lenCBFn) == FAILURE) {
    printf("Failed\n");
    return;
  }
  urlStr[lenURL] = 0;
  wv.url = urlStr;
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

  char *scr = NULL;
  int lenScr;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &scr, &lenScr) == FAILURE) {
    printf("Failed\n");
    return;
  }
  scr[lenScr] = 0;
  webview_eval(&wv, scr);

}

void my_cb(struct webview *w, const char *arg) {
  //char msg[1000];
  //printf("Hello: %s\n", arg);
  //snprintf(msg, sizeof(msg), "alert('%s')", arg);

  zval p1;
  INIT_ZVAL(p1);
  ZVAL_STRING(&p1, arg, 1);
  zval *params = { &p1 };
  zend_uint param_count = 1;
  zval retval;

  zval function_name;
  INIT_ZVAL(function_name);
  ZVAL_STRING(&function_name, cbFnStr, 1);

  if (call_user_function(CG(function_table), NULL, // no object
        &function_name, &retval, param_count,
        &params TSRMLS_CC) == SUCCESS) {
      //printf("Success returning from PHP\n");
      if (Z_TYPE_P(&retval) == IS_STRING) {
        char *cstr;
        int cstrlen;
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
