#include "hard.h"
#include "drawGrid.h"
#include "drawNumbers.h"
#include "puzzle_loader.h"
#include <random>
#include <optional>
#include "checkBoard.h"


static void showLoadError(sf::RenderWindow& window)
{
    // ostani na ekranu da vidiš title, izađi na ESC ili close
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }
            if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
                if (kp->code == sf::Keyboard::Key::Escape) return;
        }

        window.clear(sf::Color(80, 0, 0)); // tamno crveno
        window.display();
    }
}
static std::optional<int> cellFromMouse(sf::Vector2f m, const GridLayout& L)
{
    if (m.x < L.origin.x || m.y < L.origin.y) return std::nullopt;
    if (m.x >= L.origin.x + L.size || m.y >= L.origin.y + L.size) return std::nullopt;
    int c = static_cast<int>((m.x - L.origin.x) / L.cell);
    int r = static_cast<int>((m.y - L.origin.y) / L.cell);
    if (r < 0 || r > 8 || c < 0 || c > 8) return std::nullopt;
    return r * 9 + c; // 0..80
}

void startHard(sf::RenderWindow& window, const sf::Font& uiFont, const sf::Font& gridFont)
{
    bool solved = false;
    bool gameOver = false;

    window.setTitle("hard: usao u starthard");

    GridLayout L;
    L.cell = 70.f;
    L.size = 9.f * L.cell;
    L.origin = {
        (static_cast<float>(window.getSize().x) - L.size) / 2.f,
        (static_cast<float>(window.getSize().y) - L.size) / 2.f
    };
    sf::RectangleShape newGameBox;
    sf::Text newGameText(uiFont);

    // confirm popup flag
    bool showNewGameConfirm = false;

    auto res = puzzle::loadFromFile("hard_puzzles.txt");

    if (!res.error.empty()) {
        window.setTitle("hard ERROR: " + res.error);
        showLoadError(window);
        return;
    }
    if (res.puzzles.empty()) {
        window.setTitle("hard ERROR: puzzles.empty()");
        showLoadError(window);
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, res.puzzles.size() - 1);
    size_t pick = dist(gen);

    puzzle::Puzzle given = res.puzzles[pick];
    puzzle::Puzzle current = given;
    std::optional<int> selected;
    bool showErrors = true;


    check::ErrorMap errors{};
    errors.fill(false);

    sf::RectangleShape backBox(sf::Vector2f(160.f, 45.f));

    backBox.setFillColor(sf::Color(40, 40, 40));
    backBox.setOutlineThickness(2.f);
    backBox.setOutlineColor(sf::Color(200, 200, 200));

    sf::Text backText(uiFont);
    backText.setString("<--- POVRATAK NA MENI");
    backText.setCharacterSize(26);
    backText.setFillColor(sf::Color::White);

    backBox.setPosition({ 20.f, 20.f });
    backBox.setSize({ 530.f, 70.f });
    auto bb = backText.getLocalBounds();
    backText.setPosition({
        backBox.getPosition().x + (backBox.getSize().x - bb.size.x) / 2.f - bb.position.x,
        backBox.getPosition().y + (backBox.getSize().y - bb.size.y) / 2.f - bb.position.y
        });
    // NEW GAME gumb (pozicioniraj kako ti paše)
    newGameBox.setSize({ 360.f, 60.f });
    newGameBox.setFillColor(sf::Color(25, 25, 28));
    newGameBox.setOutlineThickness(2.f);
    newGameBox.setOutlineColor(sf::Color(200, 200, 200));

    // primjer: gore lijevo ispod back gumba
    newGameBox.setPosition({ 50.f, 120.f });

    newGameText.setString("NEW GAME");
    newGameText.setCharacterSize(26);
    newGameText.setFillColor(sf::Color::White);
    newGameText.setStyle(sf::Text::Bold);

    auto ngb = newGameText.getLocalBounds();
    newGameText.setPosition({
        newGameBox.getPosition().x + (newGameBox.getSize().x - ngb.size.x) / 2.f - ngb.position.x,
        newGameBox.getPosition().y + (newGameBox.getSize().y - ngb.size.y) / 2.f - ngb.position.y
        });

    sf::RectangleShape checkBox(sf::Vector2f(190.f, 45.f));

    checkBox.setFillColor(sf::Color(40, 40, 40));
    checkBox.setOutlineThickness(2.f);
    checkBox.setOutlineColor(sf::Color(200, 200, 200));
    const float margin = 20.f;
    checkBox.setSize({ 280.f, 70.f }); // ti biraj
    checkBox.setPosition({
        L.origin.x + (L.size - checkBox.getSize().x) / 2.f, // centrirano po širini grida
        L.origin.y + L.size + margin                         // ispod grida
        });
    sf::Text checkText(uiFont);
    checkText.setFillColor(sf::Color::White);
    checkText.setCharacterSize(30);
    checkText.setString("Provjera (max 10)");


    auto cb = checkText.getLocalBounds();
    checkText.setPosition({
        checkBox.getPosition().x + (checkBox.getSize().x - cb.size.x) / 2.f - cb.position.x,
        checkBox.getPosition().y + (checkBox.getSize().y - cb.size.y) / 2.f - cb.position.y
        });

    //ZA POP UP
    bool showFillAllPopup = false;
    bool showBackConfirm = false;
    bool goBackToMenu = false;
    //counter za greske
    int mistakesMade = 0;




    auto refreshGame = [&]() {
        current = given;
        selected.reset();
        solved = false;

        gameOver = false;
        mistakesMade = 0;

        errors = check::computeErrors(current); // ili errors.fill(false); ali ovo je ok
        showErrors = true;
        };


    while (window.isOpen())
    {
        // 1) EVENTS
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }

            if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Escape) return;

                if (selected.has_value())
                {
                    if (gameOver) continue;
                    int idx = *selected;

                    if (given[idx] == 0) // samo ako nije zadani broj
                    {
                        if (kp->code == sf::Keyboard::Key::Backspace ||
                            kp->code == sf::Keyboard::Key::Delete ||
                            kp->code == sf::Keyboard::Key::Num0 ||
                            kp->code == sf::Keyboard::Key::Numpad0)
                        {
                            current[idx] = 0;
                            solved = false;
                            errors = check::computeErrors(current);


                            continue;

                        }

                        auto setDigit = [&](int d)
                            {
                                if (gameOver) return;

                                current[idx] = static_cast<std::uint8_t>(d);
                                solved = false;

                                errors = check::computeErrors(current);

                                // kaznena greška: ako je potez napravio konflikt -> +1
                                if (errors[idx]) {
                                    mistakesMade++;
                                    if (mistakesMade >= 10) {
                                        gameOver = true;
                                        return;
                                    }
                                }

                                // riješeno: nema grešaka na ploči + nema praznih polja
                                bool anyError = false;
                                for (int i = 0; i < 81; i++) {
                                    if (errors[i]) { anyError = true; break; }
                                }

                                if (!anyError) {
                                    solved = true;
                                    for (int i = 0; i < 81; i++) {
                                        if (current[i] == 0) { solved = false; break; }
                                    }
                                }
                            };




                        if (kp->code == sf::Keyboard::Key::Num1) setDigit(1);
                        else if (kp->code == sf::Keyboard::Key::Num2) setDigit(2);
                        else if (kp->code == sf::Keyboard::Key::Num3) setDigit(3);
                        else if (kp->code == sf::Keyboard::Key::Num4) setDigit(4);
                        else if (kp->code == sf::Keyboard::Key::Num5) setDigit(5);
                        else if (kp->code == sf::Keyboard::Key::Num6) setDigit(6);
                        else if (kp->code == sf::Keyboard::Key::Num7) setDigit(7);
                        else if (kp->code == sf::Keyboard::Key::Num8) setDigit(8);
                        else if (kp->code == sf::Keyboard::Key::Num9) setDigit(9);

                        else if (kp->code == sf::Keyboard::Key::Numpad1) setDigit(1);
                        else if (kp->code == sf::Keyboard::Key::Numpad2) setDigit(2);
                        else if (kp->code == sf::Keyboard::Key::Numpad3) setDigit(3);
                        else if (kp->code == sf::Keyboard::Key::Numpad4) setDigit(4);
                        else if (kp->code == sf::Keyboard::Key::Numpad5) setDigit(5);
                        else if (kp->code == sf::Keyboard::Key::Numpad6) setDigit(6);
                        else if (kp->code == sf::Keyboard::Key::Numpad7) setDigit(7);
                        else if (kp->code == sf::Keyboard::Key::Numpad8) setDigit(8);
                        else if (kp->code == sf::Keyboard::Key::Numpad9) setDigit(9);
                    }
                }
            }

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mb->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f m = window.mapPixelToCoords({ mb->position.x, mb->position.y });
                    if (showFillAllPopup) {
                        showFillAllPopup = false;
                        continue; // pojede klik, da ne klikneš nešto ispod
                    }


                    // ⬇️ 0) AKO JE CONFIRM POPUP OTVOREN
                    if (showBackConfirm) {

                        sf::Vector2f win(window.getSize());
                        sf::Vector2f popSize(520.f, 200.f);
                        sf::Vector2f popPos(
                            win.x / 2.f - popSize.x / 2.f,
                            win.y / 2.f - popSize.y / 2.f
                        );

                        sf::FloatRect yesRect(
                            sf::Vector2f(popPos.x + 90.f, popPos.y + 135.f),
                            sf::Vector2f(140.f, 45.f)
                        );

                        sf::FloatRect noRect(
                            sf::Vector2f(popPos.x + 290.f, popPos.y + 135.f),
                            sf::Vector2f(140.f, 45.f)
                        );


                        if (yesRect.contains(m)) {

                            return; // ili state = MENU;
                        }

                        if (noRect.contains(m)) {
                            showBackConfirm = false;
                            continue;
                        }

                        // klik bilo gdje drugdje = zatvori popup
                        showBackConfirm = false;
                        continue;

                    }

                    // 1) BACK
                    if (backBox.getGlobalBounds().contains(m)) {
                        showBackConfirm = true;
                        continue; // ne radi ništa drugo u tom kliku
                    }



                    if (showNewGameConfirm) {
                        sf::Vector2u ws = window.getSize();
                        sf::Vector2f winSize{ (float)ws.x, (float)ws.y };

                        sf::Vector2f popSize{ 620.f, 220.f };
                        sf::Vector2f popPos{ winSize.x / 2.f - popSize.x / 2.f, winSize.y / 2.f - popSize.y / 2.f };

                        sf::FloatRect yesRect(
                            sf::Vector2f(popPos.x + 120.f, popPos.y + 150.f),
                            sf::Vector2f(160.f, 50.f)
                        );

                        sf::FloatRect noRect(
                            sf::Vector2f(popPos.x + 340.f, popPos.y + 150.f),
                            sf::Vector2f(160.f, 50.f)
                        );


                        if (yesRect.contains(m)) {
                            showNewGameConfirm = false;
                            refreshGame();     //“refresh”
                            continue;
                        }
                        if (noRect.contains(m)) {
                            showNewGameConfirm = false;
                            continue;
                        }

                        continue; // pojedi klik da ne klikne ispod
                    }
                    if (newGameBox.getGlobalBounds().contains(m)) {
                        showNewGameConfirm = true;
                        continue;
                    }


                    // 3) klik na tablu => selekcija celije
                    selected = cellFromMouse(m, L);
                }
            }


            // 2) DRAW  ✅ OVO MORA BITI OVDJE
            window.clear(sf::Color(35, 35, 35));
            drawGrid(window, L);
            if (showErrors)
            {
                sf::Color errBg(120, 40, 40); // tamno crveno
                for (int idx = 0; idx < 81; idx++) {
                    if (!errors[idx]) continue;

                    int r = idx / 9;
                    int c = idx % 9;

                    sf::RectangleShape cell(sf::Vector2f(L.cell, L.cell));
                    cell.setPosition({ L.origin.x + c * L.cell, L.origin.y + r * L.cell });
                    cell.setFillColor(errBg);
                    window.draw(cell);
                }
            }

            if (selected.has_value())
            {
                int idx = *selected;
                int r = idx / 9;
                int c = idx % 9;

                sf::RectangleShape sel(sf::Vector2f(L.cell, L.cell));
                sel.setPosition({ L.origin.x + c * L.cell, L.origin.y + r * L.cell });
                sel.setFillColor(sf::Color(70, 70, 90));
                window.draw(sel);




            }

            window.draw(backBox);
            window.draw(backText);





            drawNumbers(window, L, given, current, uiFont);
            if (solved)
            {
                sf::RectangleShape overlay(
                    sf::Vector2f(
                        static_cast<float>(window.getSize().x),
                        static_cast<float>(window.getSize().y)
                    )
                );
                overlay.setFillColor(sf::Color(40, 140, 60, 200));
                window.draw(overlay);

                sf::Text msg(uiFont);
                msg.setString("USPJESNO RIJESENO!");
                msg.setCharacterSize(64);
                msg.setFillColor(sf::Color::White);
                msg.setStyle(sf::Text::Bold);

                auto b = msg.getLocalBounds();
                msg.setPosition({
                    window.getSize().x / 2.f - b.size.x / 2.f - b.position.x,
                    window.getSize().y / 2.f - b.size.y / 2.f - b.position.y
                    });

                window.draw(msg);
            }

            if (gameOver)
            {
                sf::RectangleShape overlay(
                    sf::Vector2f(
                        static_cast<float>(window.getSize().x),
                        static_cast<float>(window.getSize().y)
                    )
                );
                overlay.setFillColor(sf::Color(140, 40, 40, 200));
                window.draw(overlay);

                sf::Text msg(uiFont);
                msg.setString("GAME OVER");
                msg.setCharacterSize(72);
                msg.setFillColor(sf::Color::Red);
                msg.setStyle(sf::Text::Bold);

                auto b = msg.getLocalBounds();
                msg.setPosition({
                    window.getSize().x / 2.f - b.size.x / 2.f - b.position.x,
                    window.getSize().y / 2.f - b.size.y / 2.f - b.position.y
                    });

                window.draw(msg);
            }


            if (showBackConfirm) {
                // window size -> float
                sf::Vector2u ws = window.getSize();
                sf::Vector2f winSize{ static_cast<float>(ws.x), static_cast<float>(ws.y) };

                // dim background
                sf::RectangleShape dim;
                dim.setSize(winSize);
                dim.setFillColor(sf::Color(0, 0, 0, 120));
                window.draw(dim);

                // popup panel
                sf::RectangleShape pop;
                pop.setSize({ 520.f, 200.f });
                pop.setFillColor(sf::Color(30, 30, 30, 230));
                pop.setOutlineThickness(2.f);
                pop.setOutlineColor(sf::Color(220, 220, 220));
                pop.setPosition({ winSize.x / 2.f - 260.f, winSize.y / 2.f - 100.f });
                window.draw(pop);

                // message
                sf::Text msg(uiFont);
                msg.setString("Jeste li sigurni?\nIzgubit ce se vas unos.");
                msg.setCharacterSize(24);
                msg.setFillColor(sf::Color::White);

                auto mb = msg.getLocalBounds();
                msg.setPosition({
                    pop.getPosition().x + (pop.getSize().x - mb.size.x) / 2.f - mb.position.x,
                    pop.getPosition().y + 25.f
                    });
                window.draw(msg);

                // YES button
                sf::RectangleShape yes;
                yes.setSize({ 140.f, 45.f });
                yes.setFillColor(sf::Color(70, 140, 70));
                yes.setPosition({ pop.getPosition().x + 90.f, pop.getPosition().y + 135.f });
                window.draw(yes);

                sf::Text yesT(uiFont);
                yesT.setString("DA");
                yesT.setCharacterSize(22);
                yesT.setFillColor(sf::Color::White);
                auto yb = yesT.getLocalBounds();
                yesT.setPosition({
                    yes.getPosition().x + (yes.getSize().x - yb.size.x) / 2.f - yb.position.x,
                    yes.getPosition().y + (yes.getSize().y - yb.size.y) / 2.f - yb.position.y
                    });
                window.draw(yesT);

                // NO button
                sf::RectangleShape no;
                no.setSize({ 140.f, 45.f });
                no.setFillColor(sf::Color(150, 70, 70));
                no.setPosition({ pop.getPosition().x + 290.f, pop.getPosition().y + 135.f });
                window.draw(no);

                sf::Text noT(uiFont);
                noT.setString("NE");
                noT.setCharacterSize(22);
                noT.setFillColor(sf::Color::White);
                auto nb = noT.getLocalBounds();
                noT.setPosition({
                    no.getPosition().x + (no.getSize().x - nb.size.x) / 2.f - nb.position.x,
                    no.getPosition().y + (no.getSize().y - nb.size.y) / 2.f - nb.position.y
                    });
                window.draw(noT);
            }

            sf::Text errTxt(uiFont);
            errTxt.setCharacterSize(32);                 // VEĆE
            errTxt.setFillColor(sf::Color::White);       // BIJELO
            errTxt.setStyle(sf::Text::Bold);             // BOLD (opcionalno)

            errTxt.setString("GRESKE: " + std::to_string(mistakesMade) + " / 10");

            // centriraj iznad sudokua
            auto eb = errTxt.getLocalBounds();
            float x = L.origin.x + (L.size - eb.size.x) / 2.f - eb.position.x;
            float y = L.origin.y - 50.f;                 // malo iznad grida (podesi 40-70)

            errTxt.setPosition({ x, y });

            window.draw(errTxt);

            window.draw(newGameBox);
            window.draw(newGameText);

            if (showNewGameConfirm) {
                sf::Vector2u ws = window.getSize();
                sf::Vector2f winSize{ (float)ws.x, (float)ws.y };

                sf::RectangleShape dim;
                dim.setSize(winSize);
                dim.setFillColor(sf::Color(0, 0, 0, 140));
                window.draw(dim);

                sf::RectangleShape pop;
                pop.setSize({ 620.f, 220.f });
                pop.setFillColor(sf::Color(25, 25, 28, 240));
                pop.setOutlineThickness(2.f);
                pop.setOutlineColor(sf::Color(220, 220, 220));
                pop.setPosition({ winSize.x / 2.f - 310.f, winSize.y / 2.f - 110.f });
                window.draw(pop);

                sf::Text msg(uiFont);
                msg.setString("Jeste li sigurni?\nNova igra brise trenutni unos.");
                msg.setCharacterSize(24);
                msg.setFillColor(sf::Color::White);

                auto mb = msg.getLocalBounds();
                msg.setPosition({
                    pop.getPosition().x + (pop.getSize().x - mb.size.x) / 2.f - mb.position.x,
                    pop.getPosition().y + 30.f
                    });
                window.draw(msg);

                sf::RectangleShape yes;
                yes.setSize({ 160.f, 50.f });
                yes.setFillColor(sf::Color(70, 140, 70));
                yes.setPosition({ pop.getPosition().x + 120.f, pop.getPosition().y + 150.f });
                window.draw(yes);

                sf::Text yesT(uiFont);
                yesT.setString("DA");
                yesT.setCharacterSize(24);
                yesT.setFillColor(sf::Color::White);
                auto yb = yesT.getLocalBounds();
                yesT.setPosition({
                    yes.getPosition().x + (yes.getSize().x - yb.size.x) / 2.f - yb.position.x,
                    yes.getPosition().y + (yes.getSize().y - yb.size.y) / 2.f - yb.position.y
                    });
                window.draw(yesT);

                sf::RectangleShape no;
                no.setSize({ 160.f, 50.f });
                no.setFillColor(sf::Color(150, 70, 70));
                no.setPosition({ pop.getPosition().x + 340.f, pop.getPosition().y + 150.f });
                window.draw(no);

                sf::Text noT(uiFont);
                noT.setString("NE");
                noT.setCharacterSize(24);
                noT.setFillColor(sf::Color::White);
                auto nb = noT.getLocalBounds();
                noT.setPosition({
                    no.getPosition().x + (no.getSize().x - nb.size.x) / 2.f - nb.position.x,
                    no.getPosition().y + (no.getSize().y - nb.size.y) / 2.f - nb.position.y
                    });
                window.draw(noT);
            }

            window.display();
        }
    }
}





