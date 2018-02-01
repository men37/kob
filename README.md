# kob

For Gnu/Linux Only.

The idea is each character you want the program to understand is parsed as some decimal number you give to it.
But the decimal numbers get randomly reassigned for each character as you go.

In total, the lowercase english alphabet, characters '0' through '9', and the space key get reassigned different numerical values after each time you pick a key.


Randomness is seeded using your system's random device.

The reassignments are presented as "letter here=decimal value here".
The table gets cleared every time after you input a value.

It is of note each character of a password is encrypted after you provide it as well and then remains protected using mprotect until your whole password entry is finished.

A file you selected to load at the beginning will then have applied an XOR operation repeatedly using your password. Finally the file is saved over the one the original.

At this time the options obfuscate and deobfuscate are really the same effect.
