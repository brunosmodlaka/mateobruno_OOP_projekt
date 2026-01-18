#include "drawNumbers.h"
#include <string>

void drawNumbers(
    sf::RenderWindow& window,
    const GridLayout& L,
    const puzzle::Puzzle& given,
    const puzzle::Puzzle& current,
    const sf::Font& font
) {
    sf::Color givenColor(240, 240, 240);
    sf::Color userColor(180, 210, 255);

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {

            int idx = r * 9 + c;
            std::uint8_t v = current[idx];
            if (v == 0) continue;

            sf::Text t(font);
            t.setString(std::to_string((int)v));
            t.setCharacterSize(static_cast<unsigned>(L.cell * 0.60f));

            t.setFillColor(given[idx] != 0 ? givenColor : userColor);

            auto b = t.getLocalBounds();
            float x = L.origin.x + c * L.cell + (L.cell - b.size.x) / 2.f - b.position.x;
            float y = L.origin.y + r * L.cell + (L.cell - b.size.y) / 2.f - b.position.y;
            t.setPosition({ x, y });

            window.draw(t);
        }
    }
}
