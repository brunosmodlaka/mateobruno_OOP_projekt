#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace puzzle {

    using Puzzle = std::array<std::uint8_t, 81>; // 0..9

    struct LoadResult {
        std::vector<Puzzle> puzzles;
        std::string error; // prazno ako je sve ok
    };

    // Ucita sve puzzle iz txt fajla.
    // Format: svaka tabla = 9 linija, svaka linija = 9 znakova (0 ili . za prazno, 1..9 brojevi).
    LoadResult loadFromFile(const std::string& path);

} // namespace puzzle
