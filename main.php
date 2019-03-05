<?php

function cb_from_webview($arg) {
  webview_eval("document.write('First script run from PHP')");
  echo "In PHP:", $arg, "\n";
  return "var dv = document.createElement('div'); dv.innerHTML='Second script run from PHP'; document.body.append(dv);";
}

webview("file://" . getcwd() . "/index.html", "cb_from_webview");

?>
