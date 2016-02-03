PHP Hunspell Dictionary
=======================

Provides an API for [Hunspell](https://hunspell.github.io/).

> Hunspell is the spell checker of LibreOffice, OpenOffice.org, Mozilla Firefox 3 & Thunderbird, Google Chrome, and it is also used by proprietary software packages, like Mac OS X, InDesign, memoQ, Opera and SDL Trados.

# Installation

```
phpize
./configure
make
sudo make install
```

# API

## `Hunspell::__construct($affix, $dict)`

Creates a new instance of `Hunspell`, providing path to the affix and dictionary file.

## `bool Hunspell::add($word)`

Adds a custom word to the dictionary.

## `bool Hunspell::addDictionary($dict)`

Adds a dictionary to the session, providing path to the dictionary file.

## `array Hunspell::analyze($word)`

Performs morphological analysis on the word.

## `array Hunspell::generate($word1, $word2)`

Declensions can be generated by providing a sample to the `generate` method.

## `bool Hunspell::spell($word)`

Checks if a word is spelled correctly.

## `array Hunspell::suggest($word)`

Retrieve suggestions for misspelled word.

## `array Hunspell::stem($word)`

Returns the stems for a word.
