#include "Popup.h"

Popup::Popup(const sf::Font& font, sf::Vector2u windowSize)
    : text(font), duration(2.f), visible(false)   
{
    background.setSize({ 500.f, 120.f });
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOrigin(background.getSize() / 2.f);
    background.setPosition({ windowSize.x / 2.f, windowSize.y / 2.f });

    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
}

void Popup::show(const std::string& message)
{
    text.setString(message);

    auto bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    text.setPosition(background.getPosition());

    timer.restart();
    visible = true;
}

void Popup::update()
{
    if (visible && timer.getElapsedTime().asSeconds() > duration)
        visible = false;
}

void Popup::draw(sf::RenderWindow& window)
{
    if (!visible) return;

    window.draw(background);
    window.draw(text);
}
