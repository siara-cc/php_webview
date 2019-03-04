<?

function cb_from_webview($arg) {
  webview_eval("document.write('Eval')");
  echo "In PHP:", $arg, "\n";
  return "alert('hello');";
}

webview("file://" . getcwd() . "/index.html", "cb_from_webview");

?>
