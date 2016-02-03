--TEST--
Test begin
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

$hspell = new Hunspell();

?>
--EXPECT--
Warning: Hunspell::__construct() expects exactly 2 parameters, 0 given in /Users/david/salt/hunspell-ext/tests/001.php on line 3
