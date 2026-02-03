#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Popup
{
private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Clock timer;
    float duration;
    bool visible;

public:
    Popup(const sf::Font& font, sf::Vector2u windowSize);

    void show(const std::string& message);
    void update();
    void draw(sf::RenderWindow& window);
};
