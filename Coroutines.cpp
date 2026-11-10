#include "Coroutines.h"

using namespace std;

static void narrow_range(int& low, int& high, int guess, int reaction)
{
    if (reaction < 0)
        low = guess + 1;
    else if (reaction > 0)
        high = guess - 1;
}

Guesser guess_number(int low, int high, shared_ptr<int> reaction)
{
    bool guessed = false;
    while (!guessed && low <= high)
    {
        int guess = low + (high - low) / 2;
        co_yield guess;
        guessed = *reaction == 0;
        narrow_range(low, high, guess, *reaction);
    }
    co_return guessed;
}