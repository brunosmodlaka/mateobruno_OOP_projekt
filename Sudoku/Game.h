#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Sudoku.h"
#include "Popup.h"

class Igra
{
private:
    Sudoku sudoku;
    int selRow;
    int selCol;
    int greska;
    int maxGreska;
    Popup popup;

public:
    Igra(const sf::Font& font, sf::Vector2u windowSize);

    void OznaceniKvadratic(int row, int col);
    bool UneseniBroj(int num);
    const Sudoku& getSudoku() const;
    int getSelectedRow() const;
    int getSelectedCol() const;
    int getMistakes() const;
    bool isGameOver() const;
    bool isWin() const;
    void updatePopup();
    void drawPopup(sf::RenderWindow& window);
};

#endif
