#include "Game.h"

Igra::Igra(const sf::Font& font, sf::Vector2u windowSize)
    : popup(font, windowSize)
{
    selRow = -1;
    selCol = -1;
    greska = 0;
    maxGreska = 5;
}

void Igra::OznaceniKvadratic(int row, int col)
{
    selRow = row;
    selCol = col;
}

bool Igra::UneseniBroj(int num)
{
    if (selRow == -1 || selCol == -1)
        return false;

    if (!sudoku.setNumber(selRow, selCol, num))
    {
        greska++;
        popup.show("Neispravan unos - greska pronadjena");
        return false;
    }

    return true;
}


const Sudoku& Igra::getSudoku() const
{
    return sudoku;
}

int Igra::getSelectedRow() const
{
    return selRow;
}

int Igra::getSelectedCol() const
{
    return selCol;
}

int Igra::getMistakes() const
{
    return greska;
}

bool Igra::isGameOver() const
{
    return greska >= maxGreska;
}

bool Igra::isWin() const
{
    return sudoku.isComplete();
}
void Igra::updatePopup()
{
    popup.update();
}

void Igra::drawPopup(sf::RenderWindow& window)
{
    popup.draw(window);
}