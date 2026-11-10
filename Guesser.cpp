#include "Guesser.h"
#include <exception>
#include <stdexcept>

using namespace std;

Guesser::promise_type::promise_type(int, int, shared_ptr<int> reaction)
    : reaction(reaction)
{
}

Guesser Guesser::promise_type::get_return_object()
{
    return Guesser{ handle::from_promise(*this) };
}

suspend_always Guesser::promise_type::initial_suspend()
{
    return suspend_always{};
}

suspend_always Guesser::promise_type::final_suspend() noexcept
{
    return suspend_always{};
}

void Guesser::promise_type::unhandled_exception()
{
    terminate();
}

suspend_always Guesser::promise_type::yield_value(int value)
{
    guess = value;
    return suspend_always{};
}

void Guesser::promise_type::return_value(bool result)
{
    guessed = result;
}


Guesser::Guesser(handle h) : coro(h)
{
}

Guesser::Guesser(Guesser&& other) noexcept : coro(other.coro)
{
    other.coro = nullptr;
}

Guesser::~Guesser()
{
    if (coro)
        coro.destroy();
}

bool Guesser::in_progress() const
{
    return coro && !coro.done();
}

bool Guesser::move_next()
{
    if (!in_progress())
        return false;
    coro.resume();
    return !coro.done();
}

int Guesser::current_guess() const
{
    if (!in_progress())
        throw logic_error("no current guess: game is over");
    return coro.promise().guess;
}

void Guesser::answer(int reaction)
{
    if (!in_progress())
        throw logic_error("cannot answer: game is over");
    *coro.promise().reaction = reaction;
}

bool Guesser::is_guessed() const
{
    if (!coro || !coro.done())
        throw logic_error("game is not finished yet");
    return coro.promise().guessed;
}