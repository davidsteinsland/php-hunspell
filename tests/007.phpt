--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->add('PQL'));
var_dump($hspell->add('PHP'));

var_dump($hspell->spell('PQL'));
var_dump($hspell->spell('PHP'));

var_dump($hspell->suggest('PHQ'));

unset($hspell);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
array(1) {
  [0]=>
  string(3) "PHP"
}
