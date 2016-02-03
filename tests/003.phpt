--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->checkSpelling('Kontor'));
var_dump($hspell->suggestSpelling('Kontor'));

unset($hspell);
?>
--EXPECT--
bool(true)
array(12) {
  [0]=>
  string(6) "Kontor"
  [1]=>
  string(5) "Konto"
  [2]=>
  string(5) "Kontr"
  [3]=>
  string(7) "Kontoer"
  [4]=>
  string(7) "Kontors"
  [5]=>
  string(7) "Kontora"
  [6]=>
  string(6) "Konter"
  [7]=>
  string(6) "Kantor"
  [8]=>
  string(6) "Kondor"
  [9]=>
  string(6) "Kontur"
  [10]=>
  string(7) "Kont or"
  [11]=>
  string(7) "Kont-or"
}
