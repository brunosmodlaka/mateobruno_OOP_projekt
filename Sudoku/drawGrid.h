#pragma once
#include <SFML/Graphics.hpp>

struct GridLayout {
    sf::Vector2f origin; // top-left table
    float size;          // ukupna velicina (npr 9*cell)
    float cell;          // velicina jedne celije
};

void drawGrid(sf::RenderWindow& window, const GridLayout& L);
