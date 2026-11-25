#include <SFML/Graphics.hpp>
#include <optional>
#include "drawGrid.h"
#include "Game.h"
#include "Popup.h"

int main()
{
    sf::VideoMode dm = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(dm, "Sudoku");

    float cell = 60.f;
    float size = cell * 9.f;

    float w = (float)dm.size.x;
    float h = (float)dm.size.y;

    GridLayout layout;
    layout.cell = cell;
    layout.size = size;
    layout.origin = {
        w / 2.f - size / 2.f,
        h / 2.f - size / 2.f
    };
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
        return -1;

    Popup popup(font, window.getSize());
    Igra igra(font, window.getSize());
    // ovo je vecinski vamo sfml tekstovi itd....
    sf::Text tekstGreske(font);
    tekstGreske.setCharacterSize(28);
    tekstGreske.setFillColor(sf::Color::White);
    tekstGreske.setPosition({ 30.f, 30.f });

    sf::RectangleShape gameOverBg;
    gameOverBg.setSize({
        (float)window.getSize().x,
        (float)window.getSize().y
        });
    gameOverBg.setFillColor(sf::Color(150, 0, 0));
    sf::Text gameOverText(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::White);
    
    sf::Text winText(font);
    winText.setString("YOU WIN");
    winText.setCharacterSize(72);
    winText.setFillColor(sf::Color::White);
    auto centerText = [&](sf::Text& t)
        {
            auto b = t.getLocalBounds();
            t.setPosition({
                window.getSize().x / 2.f - b.size.x / 2.f - b.position.x,
                window.getSize().y / 2.f - b.size.y / 2.f - b.position.y
                });
        };

    centerText(gameOverText);
    centerText(winText);

    while (window.isOpen())
    {
        while (auto e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window.close();

            
            if (const auto* klik = e->getIf<sf::Event::MouseButtonPressed>())
            {
                window.requestFocus();

                float mx = klik->position.x - layout.origin.x;
                float my = klik->position.y - layout.origin.y;

                if (mx >= 0 && my >= 0 &&
                    mx < layout.size && my < layout.size)
                {
                    int stupac = int(mx / layout.cell);
                    int red = int(my / layout.cell);
                    igra.OznaceniKvadratic(red, stupac);
                }
            }

            
            if (const auto* txt = e->getIf<sf::Event::TextEntered>())
            {
                int u = txt->unicode;

                if (u >= 49 && u <= 57)
                    igra.UneseniBroj(u - 48);
                else if (u == 48)
                    igra.UneseniBroj(0);
            }
        }

        

        window.clear();

        if (igra.isGameOver())
        {
            window.clear(sf::Color(150, 0, 0)); 
            window.draw(gameOverText);
        }
        else if (igra.isWin())
        {
            window.clear(sf::Color(0, 120, 0));
            window.draw(winText);
        }
        else
        {
            window.clear(sf::Color(20, 20, 20));

            drawGrid(window, layout);

            const Sudoku& s = igra.getSudoku();

            for (int r = 0; r < 9; r++)
            {
                for (int c = 0; c < 9; c++)
                {
                    int v = s.getCell(r, c);
                    if (v == 0)
                        continue;

                    sf::Text t(font);
                    t.setString(std::to_string(v));
                    t.setCharacterSize(32);
                    t.setFillColor(sf::Color::White);

                    float x = layout.origin.x + c * layout.cell + layout.cell / 2.f;
                    float y = layout.origin.y + r * layout.cell + layout.cell / 2.f;

                    auto b = t.getLocalBounds();
                    t.setPosition({
                        x - b.size.x / 2.f - b.position.x,
                        y - b.size.y / 2.f - b.position.y
                        });

                    window.draw(t);
                }
            }

            tekstGreske.setString(
                "Greske: " + std::to_string(igra.getMistakes()) + " / 5"
            );
            window.draw(tekstGreske);
        }
        igra.updatePopup();
        igra.drawPopup(window);
        window.display();

    }


    

    return 0;
}
    
