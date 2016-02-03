--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell(dirname(__DIR__) . '/dicts/nb_NO.aff', dirname(__DIR__) . '/dicts/nb_NO.dic');

var_dump($hspell->checkSpelling(null));
var_dump($hspell->suggestSpelling(null));

unset($hspell);
?>
--EXPECT--
bool(false)
bool(false)
