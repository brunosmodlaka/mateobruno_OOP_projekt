#pragma once
#include <array>
#include <cstdint>
#include "puzzle_loader.h"

namespace check {

    // true = ova celija je u gresci
    using ErrorMap = std::array<bool, 81>;

    // napravi mapu gresaka za trenutnu tablu (ignorise nule)
    ErrorMap computeErrors(const puzzle::Puzzle& board);

    // helper: da li ima ijedne greske
    bool anyError(const ErrorMap& e);
    int provjeranule(puzzle::Puzzle& board);

} // namespace check
