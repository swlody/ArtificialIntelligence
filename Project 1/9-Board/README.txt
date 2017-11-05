Run make to compile. Run ./9Board to play and follow instructions. A more thorough write-up is contained in ../Report.pdf

Also please don't enter more than 64 characters on a line.

Use ctrl+c to quit in the middle of a game.

9BoardOrdered.c will also compile, and orders the moves according to the heuristic prior to performing alpha-beta search. This version is much slower, and doesn't increase the number of cutoffs by very much.

I rarely ran into an issue where if the computer played X, it would declare a draw on the first move. However I have not experienced it since. Very rarely the program may stop computing and not deliver an answer (CPU usage drops to zero), in this case, pressing ctrl+c sometimes returned an answer, otherwise, the program would have to be cancelled and run from the start. I have not been able to reliabliy reproduce this to diagnose.

There also might be a bug where having the computer move first makes it declare a draw on the first move.