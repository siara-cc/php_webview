# Cross Platform WebView extension for PHP-ians

This WebView extension for PHP is built upon [WebView for C and Go](https://github.com/zserge/webview) written by Serge Zaitsev.

# Why WebView for PHP

PHP has always been a server side HTML preprocessor language.  Since it has a huge deployment base and developer base, it makes sense to make it available on the Front-end to build rich and responsive desktop applications.  This repository is first step towards this.

# How it works

This repository makes WebView available as a PHP extension.  There are two functions provided by the extension: `webview()` and `webview_eval()`.

Giving below is the PHP code of `main.php` which makes the API self-explanatory:

```php
<?php

function cb_from_webview($arg) {
  webview_eval("document.write('<div id=d1>First script run from PHP</div>')");
  echo "In PHP:", $arg, "\n";
  return "document.body.innerHTML += 'Second script run from PHP';";
}

webview("file://" . getcwd() . "/index.html", "cb_from_webview");

?>
```

`cb_from_webview()` is passed as second parameter to `webview()` function, which opens the URL specified in the first parameter. Whenever the script (JS) in the HTML page (index.html) calls `window.external.invoke(arg)`, the PHP callback function (`cb_from_webview()`) is invoked with the argument.

The callback function can choose to call `webview_eval()` as shown above.  Whatever string returned by this function is run as script.  If empty string is returned, it is not evaluated.

The callback function acts as a bridge between Javascript and PHP and may be called using JSON string as argument and may indicate the context of call (such as onload, onclick etc.) and corresponding data for the request or callback.

# Usage

The procedure to build the extension differs from OS to OS.

## Mac OS

Ensure PHP 5.6+ or 7+ is installed. Clone this repository, make it current folder and run following commands:

```sh
phpize
./configure CFLAGS="-DWEBVIEW_COCOA=1 -framework WebKit"
make
```

Once the `.so` file is successfully built, you can invoke the sample by using the following command:

`php -dextension=modules/webview.so main.php`

## Windows

Building on Windows is a bit tricky. First download [PHP CLI](https://windows.php.net/downloads/releases/php-7.3.2-nts-Win32-VC15-x64.zip) corresponding to your system architecture (x86/x64) and Visual Studio version. Unzip to say `c:\php7.3.2`, Open Visual Studio command prompt, change directory to this folder.  Clone this repository under ext folder.

Download [php-sdk](https://github.com/Microsoft/php-sdk-binary-tools) and unzip into different folder (say `c:\php-sdk`).  On the command prompt, add path with `path=%path%;c:\php-sdk\bin;c:\php-sdk\msys2\usr\bin`.

Run `buildconf.bat`, `configure --disable-all --enable-cli --enable-webview`.  Verify if configure will build `webview` as shared library.  Then run `nmake`.  This will build `php_webview.dll` into the corresponding folder such as `x64\Release_TS`.

To run the example, cd to the folder such as `x64\Release_TS`, run `php -dextension=php_webview.dll c:\php7.3.2\ext\php_webview\main.php`

## Linux

The procedure to build on Linux is almost same as of MacOS except following commands (Tested with PHP 5.6 and 7.2 on Linux Mint 19).  Before running, ensure `pkg-config`, `gtk+-3.0` and `webkit2gtk-4.0` are installed using `apt`.

```sh
phpize
./configure CFLAGS="-DWEBVIEW_GTK=1 `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`"
make
```

Once the `.so` file is successfully built, you can invoke the sample by using the following command:

`php -dextension=modules/webview.so -f main.php`

# Screenshot

![](ss_php_1.png?raw=true) ![](ss_php_2.png?raw=true)
