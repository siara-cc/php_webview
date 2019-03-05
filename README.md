# Cross Platform WebView extension for PHP-ians

This WebView extension for PHP is built upon [WebView for C and Go](https://github.com/zserge/webview) written by Serge Zaitsev.

While I have very little experience with PHP, I made this to be part of a larger project.  So please excuse me if my PHP looks novice-ish.

# Usage

Mac OS:

```sh
phpize
./configure CFLAGS="-DWEBVIEW_COCOA=1 -framework WebKit"
make
```

Windows:

```sh
buildconf
./configure --disable-all -enable-webview
nmake
```
# Screenshot

| ![](ss_php_1.png?raw=true) | ![](ss_php_2.png?raw=true) |
