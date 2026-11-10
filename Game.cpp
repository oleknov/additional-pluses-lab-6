#include "Game.h"
#include "Coroutines.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Move
{
    int step;
    int guess;
    int reaction;
};

enum class Outcome
{
    Guessed,
    Contradiction,
    Interrupted
};

static bool is_valid_reaction(int value)
{
    return value >= -1 && value <= 1;
}

static bool parse_reaction(const string& line, int& reaction)
{
    istringstream iss(line);
    int value = 0;
    char extra = 0;
    bool parsed = (iss >> value) && !(iss >> extra);
    if (parsed && is_valid_reaction(value))
    {
        reaction = value;
        return true;
    }
    return false;
}

static bool read_reaction(istream& in, ostream& out, int& reaction)
{
    bool valid = false;
    bool input_alive = true;
    while (!valid && input_alive)
    {
        out << "  Your reaction (-1 / 0 / 1): ";
        string line;
        if (getline(in, line))
        {
            valid = parse_reaction(line, reaction);
            if (!valid)
                out << "  Invalid value. Please enter -1, 0 or 1." << endl;
        }
        else
        {
            input_alive = false;
        }
    }
    return valid;
}

static void print_rules(int low, int high, ostream& out)
{
    out << "=== \"Guess the number\", version 2 ===" << endl
        << "Think of an integer from " << low << " to " << high
        << ". The coroutine will try to guess it." << endl
        << "Answer each guess with:" << endl
        << "  -1 - the coroutine's number is less than yours" << endl
        << "   0 - the coroutine guessed it" << endl
        << "   1 - the coroutine's number is greater than yours" << endl
        << endl;
}

static bool run_game(Guesser& guesser, istream& in, ostream& out, vector<Move>& moves)
{
    bool input_ok = true;
    int step = 0;
    while (input_ok && guesser.move_next())
    {
        ++step;
        int guess = guesser.current_guess();
        out << "Step " << step << ". The coroutine guesses: " << guess << endl;
        int reaction = 0;
        input_ok = read_reaction(in, out, reaction);
        if (input_ok)
        {
            guesser.answer(reaction);
            moves.push_back({ step, guess, reaction });
        }
    }
    return input_ok;
}

static Outcome decide_outcome(const Guesser& guesser, bool input_ok)
{
    if (!input_ok)
        return Outcome::Interrupted;
    if (guesser.is_guessed())
        return Outcome::Guessed;
    return Outcome::Contradiction;
}

static string reaction_text(int reaction)
{
    if (reaction < 0)
        return "less than the secret number";
    if (reaction > 0)
        return "greater than the secret number";
    return "guessed";
}

static void print_protocol(const vector<Move>& moves, ostream& out)
{
    out << endl
        << "=== Game log ===" << endl;
    for (const auto& move : moves)
    {
        out << "Step " << move.step << ": guess " << move.guess
            << ", reaction " << move.reaction
            << " (" << reaction_text(move.reaction) << ")" << endl;
    }
}

static void print_outcome(Outcome outcome, const vector<Move>& moves, ostream& out)
{
    out << "=== Result ===" << endl;
    if (outcome == Outcome::Guessed)
        out << "The coroutine guessed the number " << moves.back().guess
        << " in " << moves.size() << " step(s)." << endl;
    else if (outcome == Outcome::Contradiction)
        out << "The reactions are contradictory: no number in the range matches all of them."
        << endl;
    else
        out << "The game was interrupted: the input ended." << endl;
}

void play_guess_game(int low, int high, istream& in, ostream& out)
{
    if (low > high)
        throw invalid_argument("lower bound is greater than upper bound");

    print_rules(low, high, out);
    auto guesser = guess_number(low, high, make_shared<int>(0));
    vector<Move> moves;
    bool input_ok = run_game(guesser, in, out, moves);
    Outcome outcome = decide_outcome(guesser, input_ok);
    print_protocol(moves, out);
    print_outcome(outcome, moves, out);
}