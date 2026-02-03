#include "puzzle_loader.h"
#include <random>
#include <fstream>
#include <string>

namespace puzzle {

    static void rtrim(std::string& s) {
        while (!s.empty()) {
            char ch = s.back();
            if (ch == '\r' || ch == ' ' || ch == '\t') s.pop_back();
            else break;
        }
    }

    static bool parseLine9(const std::string& line, std::array<std::uint8_t, 9>& outRow) {
        if (line.size() < 9) return false;

        for (int i = 0; i < 9; i++) {
            char ch = line[i];
            if (ch == '.' || ch == '0') outRow[i] = 0;
            else if (ch >= '1' && ch <= '9') outRow[i] = static_cast<std::uint8_t>(ch - '0');
            else return false;
        }
        return true;
    }

    LoadResult loadFromFile(const std::string& path) {
        LoadResult res;

        std::ifstream in(path);
        if (!in) {
            res.error = "Ne mogu otvoriti fajl: " + path;
            return res;
        }

        Puzzle current{};
        int rowCount = 0;

        std::string line;
        int lineNo = 0;

        while (std::getline(in, line)) {
            lineNo++;
            rtrim(line);

            if (line.empty()) continue; // ignoriraj prazne linije

            std::array<std::uint8_t, 9> row{};
            if (!parseLine9(line, row)) {
                res.error = "Krivi format u fajlu " + path + " na liniji " + std::to_string(lineNo) +
                    ". Ocekivano 9 znakova (0/. ili 1-9).";
                res.puzzles.clear();
                return res;
            }

            for (int c = 0; c < 9; c++) {
                current[rowCount * 9 + c] = row[c];
            }
            rowCount++;

            if (rowCount == 9) {
                res.puzzles.push_back(current);
                rowCount = 0;            // 👇 RAZLIKA BOJE

                current.fill(0);
            }
        }

        if (rowCount != 0) {
            res.error = "Fajl " + path + " zavrsava usred table (nije kompletnih 9 linija).";
            res.puzzles.clear();
            return res;
        }

        if (res.puzzles.empty()) {
            res.error = "U fajlu " + path + " nema nijedne table (prazno ili krivi format).";
        }

        return res;
    }

} // namespace puzzle
