#ifndef PHP_WEBVIEW_H
#define PHP_WEBVIEW_H 1
#define PHP_WEBVIEW_VERSION "1.0"
#define PHP_WEBVIEW_EXTNAME "webview"

PHP_FUNCTION(webview);
PHP_FUNCTION(webview_eval);

extern zend_module_entry webview_module_entry;
#define phpext_webview_ptr &webview_module_entry

#endif
