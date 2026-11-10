#pragma once
#include <coroutine>
#include <memory>

class Guesser
{
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        int guess = 0;
        bool guessed = false;
        std::shared_ptr<int> reaction;

        promise_type(int, int, std::shared_ptr<int> reaction);

        Guesser get_return_object();
        std::suspend_always initial_suspend();
        std::suspend_always final_suspend() noexcept;
        void unhandled_exception();
        std::suspend_always yield_value(int value);
        void return_value(bool result);
    };

    Guesser(const Guesser&) = delete;
    Guesser& operator=(const Guesser&) = delete;
    Guesser(Guesser&& other) noexcept;
    ~Guesser();

    bool in_progress() const;
    bool move_next();
    int current_guess() const;
    void answer(int reaction);
    bool is_guessed() const;

private:
    Guesser(handle h);
    handle coro;
};