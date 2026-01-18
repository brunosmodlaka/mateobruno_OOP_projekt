#include "drawUserCells.h"

void drawUserCells(sf::RenderWindow& window, const GridLayout& L,
    const puzzle::Puzzle& given, const puzzle::Puzzle& current)
{
    // boja pozadine za NOVO UNEŠENE brojeve 
    sf::Color userBg(60, 70, 95); 

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int idx = r * 9 + c;

            if (given[idx] == 0 && current[idx] != 0) {
                sf::RectangleShape cell({ L.cell, L.cell });
                cell.setPosition({ L.origin.x + c * L.cell, L.origin.y + r * L.cell });
                cell.setFillColor(userBg);
                window.draw(cell);
            }
        }
    }
}
