#pragma once
#include <iostream>

// "Guess the number", version 2: the user thinks of a number in [low, high],
// the coroutine tries to guess it.
//
// Input format (stream in, one reaction per line):
//   -1  - the coroutine's number is less than the secret number;
//    0  - the coroutine guessed the number;
//    1  - the coroutine's number is greater than the secret number.
// A line with any other content is rejected and the reaction is requested again.
// End of input (Ctrl+Z on Windows, Ctrl+D on Linux) interrupts the game.
void play_guess_game(int low, int high, std::istream& in, std::ostream& out);