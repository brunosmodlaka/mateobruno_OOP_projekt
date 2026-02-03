#include <vector>
#include "Board.h"

class Sudoku
{
private:
    int grid[9][9];
    bool fixed[9][9];
    std::vector<Board> boards;

public:
    Sudoku();
    void reset();

    int getCell(int row, int col) const;
    bool isFixed(int row, int col) const;
    bool isValidMove(int row, int col, int num) const;
    bool setNumber(int row, int col, int num);
    bool isComplete() const;
};
