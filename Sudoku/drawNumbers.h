#pragma once
#include <SFML/Graphics.hpp>
#include "drawGrid.h"
#include "puzzle_loader.h"

void drawNumbers(
    sf::RenderWindow& window,
    const GridLayout& L,
    const puzzle::Puzzle& given,
    const puzzle::Puzzle& current,
    const sf::Font& font
);

