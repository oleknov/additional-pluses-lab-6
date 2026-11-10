#pragma once
#include "Guesser.h"
#include <memory>

Guesser guess_number(int low, int high, std::shared_ptr<int> reaction);