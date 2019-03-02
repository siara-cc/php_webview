
PHP_ARG_ENABLE(webview, whether to enable
WebView support,
[ --enable-webview   Enable WebView support])
if test "$PHP_WEBVIEW" = "yes"; then
  AC_DEFINE(HAVE_WEBVIEW, 1, [Whether you have WebView])
  PHP_NEW_EXTENSION(webview, webview.c, $ext_shared)
fi
