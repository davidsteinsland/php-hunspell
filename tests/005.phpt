--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->spell('Trafikkkork'));
var_dump($hspell->suggest('Trafikkkork'));
var_dump($hspell->suggest('Trafikk kork'));

unset($hspell);
?>
--EXPECT--
bool(false)
array(7) {
  [0]=>
  string(10) "Trafikkork"
  [1]=>
  string(12) "Trafikk kork"
  [2]=>
  string(12) "Trafikk-kork"
  [3]=>
  string(12) "Trafikkontor"
  [4]=>
  string(11) "Trafikkorps"
  [5]=>
  string(14) "Trafikkorridor"
  [6]=>
  string(13) "Trafikkrekord"
}
array(4) {
  [0]=>
  string(12) "Trafikk-kork"
  [1]=>
  string(10) "Trafikkork"
  [2]=>
  string(14) "Trafikkorridor"
  [3]=>
  string(12) "Trafikkontor"
}
