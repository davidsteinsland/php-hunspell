--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->analyze('Mennesker'));
var_dump($hspell->stem('Mennesker'));
var_dump($hspell->generate('jente', 'gutter'));

unset($hspell);
?>
--EXPECT--
array(2) {
  [0]=>
  string(13) " st:mennesker"
  [1]=>
  string(17) " st:menneske fl:E"
}
array(2) {
  [0]=>
  string(9) "mennesker"
  [1]=>
  string(8) "menneske"
}
array(0) {
}
