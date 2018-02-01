# kob

Alpha.

For Gnu/Linux Only.

The program does two things. 1) protects your input of your password string during operation 2) applies your password to a file and overwrites the file.

The input process works as follows: for each character you want the program to understand it must be parsed as some decimal number you give to it.

The decimal numbers will be randomly reassigned for each character as you go on.

In total, the lowercase english alphabet, characters '0' through '9', and the space key get reassigned different numerical values after each time you pick a key.

Randomness is seeded using your system's random device. If by chance your system doesn't provide a random device that collects nondeterministic value from the OS and or its use, this program becomes basically useless.

The reassignments are presented to you as "letter here=decimal value here". Input the decimal value to pick the letter (or any other desired symbol).

The table gets cleared every time after you input a value. You won't get to see what parts you've already typed out of your password.

It is of note each character of your password is encrypted after you provide it, then it remains protected using "mprotect" until your whole password entry is finished and its time to save.

A file you selected (intended to be a key file which you've already obfuscated) should load at the beginning and at the last step this file will have applied to it an XOR operation where your password gets cycled through repeatedly and used. Finally the file is saved over the one which was the original.

At this time the options obfuscate and deobfuscate have the same effect. But the program should be thought as a "secure" way to deobfuscate.

note: do not load a NON-obfuscated file. Instead of using this program to redo or make the initial obfuscation, look to externally make the obfuscation yourself or replace the deobfuscated file with a backup that is already obfuscated.
