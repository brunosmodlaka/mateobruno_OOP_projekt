#include "Sudoku.h"
#include <ctime>
#include <cstdlib>

Sudoku::Sudoku()
{
    std::srand(std::time(nullptr));

    int b1[9][9] = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };

    int b2[9][9] = {
        {0,0,0,2,6,0,7,0,1},
        {6,8,0,0,7,0,0,9,0},
        {1,9,0,0,0,4,5,0,0},
        {8,2,0,1,0,0,0,4,0},
        {0,0,4,6,0,2,9,0,0},
        {0,5,0,0,0,3,0,2,8},
        {0,0,9,3,0,0,0,7,4},
        {0,4,0,0,5,0,0,3,6},
        {7,0,3,0,1,8,0,0,0}
    };

    boards.push_back(Board(b1));
    boards.push_back(Board(b2));

    reset();
}

void Sudoku::reset()
{
    int izbor = std::rand() % boards.size();

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            grid[r][c] = boards[izbor].get(r, c);
            fixed[r][c] = (grid[r][c] != 0);
        }
    }
}

int Sudoku::getCell(int row, int col) const
{
    return grid[row][col];
}

bool Sudoku::isFixed(int row, int col) const
{
    return fixed[row][col];
}

bool Sudoku::isValidMove(int row, int col, int num) const
{
    for (int c = 0; c < 9; c++) {
        if (c != col && grid[row][c] == num) return false;
    }

    for (int r = 0; r < 9; r++) {
        if (r != row && grid[r][col] == num) return false;
    }

    int startR = (row / 3) * 3;
    int startC = (col / 3) * 3;

    for (int r = startR; r < startR + 3; r++) {
        for (int c = startC; c < startC + 3; c++) {
            if ((r != row || c != col) && grid[r][c] == num)
                return false;
        }
    }

    return true;
}

bool Sudoku::setNumber(int row, int col, int num)
{
    if (fixed[row][col]) return true;

    int old = grid[row][col];
    grid[row][col] = num;

    if (num != 0 && !isValidMove(row, col, num)) {
        grid[row][col] = old;
        return false;
    }

    return true;
}

bool Sudoku::isComplete() const
{
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (grid[r][c] == 0) return false;

    return true;
}
