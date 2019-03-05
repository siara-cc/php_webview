<?php

function cb_from_webview($arg) {
  webview_eval("document.write('<div id=d1>First script run from PHP</div>')");
  echo "In PHP:", $arg, "\n";
  return "document.body.innerHTML += 'Second script run from PHP';";
}

webview("file://" . getcwd() . "/index.html", "cb_from_webview");

?>
