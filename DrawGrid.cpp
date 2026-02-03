#include "drawGrid.h"

void drawGrid(sf::RenderWindow& window, const GridLayout& L)
{
    const int GRID = 9;
    const float CELL = L.cell;

    sf::Color blockA(45, 45, 45);
    sf::Color blockB(55, 55, 55);

    sf::Color thinLine(150, 150, 150);
    sf::Color thickLine(230, 230, 230);

    for (int r = 0; r < GRID; r++) {
        for (int c = 0; c < GRID; c++) {

            bool block = ((r / 3) + (c / 3)) % 2 == 0;
            sf::Color bg = block ? blockA : blockB;
            sf::RectangleShape cell({ CELL, CELL });
            cell.setPosition({ L.origin.x + c * CELL, L.origin.y + r * CELL });
            cell.setFillColor(bg);
            window.draw(cell);
        }
    }

    sf::RectangleShape line;
    for (int i = 0; i <= GRID; i++)
    {
        float thickness = (i % 3 == 0) ? 4.f : 1.5f;
        sf::Color color = (i % 3 == 0) ? thickLine : thinLine;

        float x = L.origin.x + i * CELL;
        line.setSize({ thickness, L.size });
        line.setPosition({ x - thickness / 2.f, L.origin.y });
        line.setFillColor(color);
        window.draw(line);

        float y = L.origin.y + i * CELL;
        line.setSize({ L.size, thickness });
        line.setPosition({ L.origin.x, y - thickness / 2.f });
        line.setFillColor(color);
        window.draw(line);
    }
}
