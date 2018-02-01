# kob

Alpha.

For Gnu/Linux Only.

The idea is each character you want the program to understand is parsed as some decimal number you give to it.
But the decimal numbers get randomly reassigned for each character as you go.

In total, the lowercase english alphabet, characters '0' through '9', and the space key get reassigned different numerical values after each time you pick a key.


Randomness is seeded using your system's random device.

The reassignments are presented as "letter here=decimal value here".
The table gets cleared every time after you input a value.

It is of note each character of a password is encrypted after you provide it as well and then remains protected using mprotect until your whole password entry is finished.

A file you selected (intended to be a key file you've already obfuscated) should load at the beginning and at the end have applied an XOR operation where your password gets cycled through repeatedly. Finally the file is saved over the one the original.

At this time the options obfuscate and deobfuscate are really the same effect.

note: do not load a sensitive file. instead of using the program to undo a change replace the sensitive key file with a backup of the protected one.
