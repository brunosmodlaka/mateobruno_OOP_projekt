#ifndef DRAWGRID_H
#define DRAWGRID_H

#include <SFML/Graphics.hpp>

struct GridLayout
{
    sf::Vector2f origin;
    float cell;
    float size;
};

void drawGrid(sf::RenderWindow& window, const GridLayout& L);

#endif
