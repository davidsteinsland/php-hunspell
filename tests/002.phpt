--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->spell('Hello'));

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/en_US.aff', dirname(__DIR__) . '/dicts/en_US.dic');
var_dump($hspell->spell('Motorize'));

var_dump($hspell->spell('Motorise'));
var_dump($hspell->suggest('Motorise'));


var_dump($hspell->spell('thrusters'));
var_dump($hspell->suggest('thrusters'));

unset($hspell);
?>
--EXPECT--
bool(false)
bool(true)
bool(false)
array(5) {
  [0]=>
  string(8) "Motorist"
  [1]=>
  string(8) "Motorize"
  [2]=>
  string(7) "Mortise"
  [3]=>
  string(10) "Servomotor"
  [4]=>
  string(8) "Tortoise"
}
bool(false)
array(2) {
  [0]=>
  string(7) "thrusts"
  [1]=>
  string(6) "thrust"
}
