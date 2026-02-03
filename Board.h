#ifndef BOARD_H
#define BOARD_H

class Board
{
private:
    int grid[9][9];

public:
    Board(int pocetno[9][9]);
    int get(int r, int c) const;
};

#endif
