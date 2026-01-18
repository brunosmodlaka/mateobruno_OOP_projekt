#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <array>
#include "easy.h"
#include "medium.h"
#include "hard.h"

enum class Difficulty { Easy, Medium, Hard };
struct Button {
    sf::RectangleShape box;      // outer
    sf::RectangleShape inner;    // inner panel
    sf::Text label;
    Difficulty value;

    Button(const sf::Font& uiFont) : label(uiFont) {}
};
    


static bool contains(const sf::RectangleShape& r, sf::Vector2f p) {
    return r.getGlobalBounds().contains(p);
}

int main() {
    sf::Font uiFont;      // retro gaming
    sf::Font gridFont;    // normalni (Arial)

    if (!uiFont.openFromFile("RetroGaming.ttf")) {
        return -1;
    }

    if (!gridFont.openFromFile("arial.ttf")) {
        return -1;
    }



    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Sudoku"
       
    );  


    auto dm = sf::VideoMode::getDesktopMode();
    const float W = static_cast<float>(dm.size.x);
    const float H = static_cast<float>(dm.size.y);

    // ---- FONT (stavi arial.ttf pored .exe) ----
 


    // Skaliranje u odnosu na 1920x1080
    const float sx = W / 1920.f;
    const float sy = H / 1080.f;
    const float s = std::min(sx, sy); // uniformno skaliranje

    // ---- NASLOVI ----
    sf::Text title(uiFont, "Dobrodosli u igru Sudoku", static_cast<unsigned>(66 * s));
    title.setFillColor(sf::Color::White);
    title.setPosition({
        W / 2.f - title.getLocalBounds().size.x / 2.f,
        90.f * s
        });

    sf::Text subtitle(uiFont, "Izaberite razinu:", static_cast<unsigned>(34 * s));
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition({
        W / 2.f - subtitle.getLocalBounds().size.x / 2.f,
        170.f * s
        });

    // ---- BUTTONI ----
    const sf::Vector2f btnSize(520.f * s, 95.f * s);
    const float startY = 290.f * s;
    const float gapY = 35.f * s;


    auto makeButton = [&](const std::string& text, float y, Difficulty d) -> Button {
        Button b(uiFont);
        b.value = d;

        // Outer box
        b.box.setSize(btnSize);
        b.box.setPosition({ W / 2.f - btnSize.x / 2.f, y });
        b.box.setFillColor(sf::Color(15, 15, 18));
        b.box.setOutlineThickness(3.f * s);
        b.box.setOutlineColor(sf::Color(180, 180, 180)); // base neon-ish

        // Inner panel (gives depth)
        b.inner.setSize({ btnSize.x - 10.f * s, btnSize.y - 10.f * s });
        b.inner.setPosition({ b.box.getPosition().x + 5.f * s, b.box.getPosition().y + 5.f * s });
        b.inner.setFillColor(sf::Color(30, 30, 36));
        b.inner.setOutlineThickness(1.f * s);
        b.inner.setOutlineColor(sf::Color(70, 70, 90));

        // Label
        b.label.setString(text);

        // pixel fonts izgledaju bolje na parnim veličinama
        unsigned charSize = static_cast<unsigned>(28 * s);
        if (charSize % 2 == 1) charSize++; // make it even
        b.label.setCharacterSize(charSize);

        b.label.setFillColor(sf::Color::White);
        b.label.setStyle(sf::Text::Bold);

        auto bounds = b.label.getLocalBounds();
        b.label.setPosition({
            b.box.getPosition().x + (btnSize.x - bounds.size.x) / 2.f - bounds.position.x,
            b.box.getPosition().y + (btnSize.y - bounds.size.y) / 2.f - bounds.position.y
            });

        return b;
        };

    std::array<Button, 3> buttons = {
        makeButton("Easy",   startY + 0.f * (btnSize.y + gapY), Difficulty::Easy),
        makeButton("Medium", startY + 1.f * (btnSize.y + gapY), Difficulty::Medium),
        makeButton("Hard",   startY + 2.f * (btnSize.y + gapY), Difficulty::Hard),
    };

    std::optional<int> hovered; // index buttona (0..2)
    std::optional<int> pressed;
    auto diffName = [](Difficulty d) -> std::string {
        switch (d) {
        case Difficulty::Easy: return "Easy";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Hard: return "Hard";
        }
        return "Medium";
        };

    while (window.isOpen()) {

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                if (kp->code == sf::Keyboard::Key::Escape) {
                    window.close();
                    break;
                }
            }

            // HOVER
            if (const auto* mm = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f mpos((float)mm->position.x, (float)mm->position.y);
                hovered.reset();
                for (int i = 0; i < (int)buttons.size(); i++) {
                    if (contains(buttons[i].box, mpos)) {
                        hovered = i;
                        break;
                    }
                }
            }

            // PRESS
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mpos((float)mb->position.x, (float)mb->position.y);

                    pressed.reset();
                    for (int i = 0; i < (int)buttons.size(); i++) {
                        if (contains(buttons[i].box, mpos)) {
                            pressed = i;

                            Difficulty chosen = buttons[i].value;
                            if (chosen == Difficulty::Easy) {
                                startEasy(window, uiFont, gridFont);
                            }
                            if (chosen == Difficulty::Medium) {
                                startMedium(window, uiFont, gridFont);
                            }
                            if (chosen == Difficulty::Hard) {
                                startHard(window, uiFont, gridFont);
                            }
                            break;
                        }
                    }
                }
            }

            
            if (const auto* mr = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mr->button == sf::Mouse::Button::Left) {
                    pressed.reset();
                }
            }
        } //  OVO zatvara pollEvent while

        //  OVDJE IDE DRAW (unutar window.isOpen while)
        window.clear(sf::Color::Black);

      
        // hover boje
        for (int i = 0; i < (int)buttons.size(); i++) {
            bool isHover = hovered.has_value() && hovered.value() == i;
            buttons[i].box.setFillColor(isHover ? sf::Color(60, 60, 90) : sf::Color(30, 30, 30));
        }

        window.draw(title);
        window.draw(subtitle);

        for (auto& b : buttons) {
            window.draw(b.box);
            window.draw(b.label);
        }

        window.display();
    }
    
    return 0;
}
