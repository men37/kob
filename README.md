# kob

For Gnu/Linux Only.

Each character you want the program to understand is parsed as some decimal number you give to it.
But the decimal numbers chosen for each character is randomized after each key is picked.

In total, the lowercase english alphabet, characters '0' through '9', and the space key get reassigned different numerical values after each time you pick a key.
Again this is done randomly.

Randomness is seeded using your system's random device.

The reassignments are presented as "letter here=decimal value here".
The table gets cleared after you input a value.

It is of note each character of a password is encrypted after you provide it and then remains protected using mprotect until your password entry is finished.

A file you selected to load at the beginning will then have applied a XOR operation repeatedly using your password. Finally the file is saved over the one that got loaded.

At this time options obfuscate and deobfuscate are really the same effect.
