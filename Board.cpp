#include "Board.h"

Board::Board(int pocetna[9][9])
{
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            grid[r][c] = pocetna[r][c];
}

int Board::get(int r, int c) const
{
    return grid[r][c];
}
