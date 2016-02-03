--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell();

?>
--EXPECTF--
Warning: Hunspell::__construct() expects exactly 2 parameters, 0 given in %s/001.php on line %d
