#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode({ 800, 600 }), "Flappy Bird - SFML 3.0.2"),
state(GameState::MainMenu), score(0), pipeSpawnTimer(0.f), lastPipeWasMoving(false),
player(nullptr) {
    window.setFramerateLimit(60);
    if (!rm.loadAll()) {
        std::exit(-1);
    }

    player = new Player(rm);

    // Initialisation des menus
    mainMenu.emplace(rm);
    optionMenu.emplace(rm);

    bg1.emplace(rm.getBgTexture());
    bg2.emplace(rm.getBgTexture());
    bgWidth = static_cast<float>(rm.getBgTexture().getSize().x);
    bg1->setPosition({ 0.f, 0.f });
    bg2->setPosition({ bgWidth, 0.f });

    menuTitle.emplace(rm.getFont(), "FLAPPY BIRD", 50);
    menuTitle->setPosition({ 230.f, 150.f });
    menuTitle->setFillColor(sf::Color::White);

    startButton.emplace(rm.getFont(), "[ Cliquez ou appuyez sur Espace pour jouer ]", 24);
    startButton->setPosition({ 170.f, 300.f });
    startButton->setFillColor(sf::Color::Yellow);

    scoreText.emplace(rm.getFont(), "Score: 0", 24);
    scoreText->setPosition({ 10.f, 10.f });
    scoreText->setFillColor(sf::Color::White);

    std::random_device rd;
    gen = std::mt19937(rd());
    gapDist = std::uniform_real_distribution<float>(100.f, 350.f);
    chanceDist = std::uniform_real_distribution<float>(0.f, 100.f);
}

void Game::resetGame() {
    player->reset();
    obstacles.clear();
    score = 0;
    scoreText->setString("Score: 0");
    pipeSpawnTimer = 0.f;
    lastPipeWasMoving = false;
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    delete player;
}

void Game::processEvents() {
    while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
        const auto& event = *eventOpt;

        if (event.is<sf::Event::Closed>()) {
            window.close();
        }

        if (state == GameState::MainMenu) {
            auto action = mainMenu->handleEvent(event);
            if (action == MainMenu::Action::Play) {
                state = GameState::Ready;
                resetGame();
            }
            else if (action == MainMenu::Action::Options) {
                state = GameState::OptionMenu;
            }
            else if (action == MainMenu::Action::Quit) {
                window.close();
            }
        }
        else if (state == GameState::OptionMenu) {
            auto action = optionMenu->handleEvent(event);
            if (action == OptionMenu::Action::Return) {
                state = GameState::MainMenu;
            }
        }
        else if (state == GameState::Ready) {
            bool startPressed = false;
            if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) startPressed = true;
            }
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) startPressed = true;
            }
            if (startPressed) {
                state = GameState::Playing;
            }
        }
        else if (state == GameState::Playing) {
            if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) player->flap();
                if ((key->code == sf::Keyboard::Key::LShift || key->code == sf::Keyboard::Key::RShift) && player->canDash()) {
                    float dashDist = player->getDashDistance();
                    bg1->move({ -dashDist, 0.f });
                    bg2->move({ -dashDist, 0.f });
                    for (auto& obs : obstacles) {
                        obs.shift(dashDist);
                    }
                    player->dash();
                }
            }
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) player->flap();
            }
        }
    }
}

void Game::update(float dt) {
    if (state == GameState::MainMenu || state == GameState::OptionMenu) {
        return; 
    }

    float bgSpeed = 100.f;
    bg1->move({ -bgSpeed * dt, 0.f });
    bg2->move({ -bgSpeed * dt, 0.f });
    if (bg1->getPosition().x <= -bgWidth) {
        bg1->setPosition({ bg2->getPosition().x + bgWidth, 0.f });
    }
    if (bg2->getPosition().x <= -bgWidth) {
        bg2->setPosition({ bg1->getPosition().x + bgWidth, 0.f });
    }

    if (state == GameState::Playing) {
        player->update(dt);
        player->updateDashCooldown(dt);

        pipeSpawnTimer += dt;
        if (pipeSpawnTimer > 1.5f) {
            bool makeMoving = false;
            if (!lastPipeWasMoving && chanceDist(gen) < 20.f) {
                makeMoving = true;
            }
            lastPipeWasMoving = makeMoving;
            obstacles.emplace_back(800.f, gapDist(gen), 150.f, rm, makeMoving);
            pipeSpawnTimer = 0.f;
        }

        bool collision = false;
        CollisionBox pBox = player->getCollisionBox();
        for (auto it = obstacles.begin(); it != obstacles.end(); ) {
            it->update(dt);
            if (pBox.intersects(it->getTopCollisionBox()) || pBox.intersects(it->getBottomCollisionBox())) {
                collision = true;
            }

            if (!it->isPassed() && it->getX() + it->getWidth() < player->getPosition().x - (pBox.getRect().size.x / 2.f)) {
                it->setPassed(true);
                score++;
                scoreText->setString("Score: " + std::to_string(score));
            }

            if (it->getX() + it->getWidth() < 0.f) {
                it = obstacles.erase(it);
            }
            else {
                ++it;
            }
        }

        if (pBox.getRect().position.y < 0.f || pBox.getRect().position.y + pBox.getRect().size.y > 600.f) {
            collision = true;
        }

        if (collision) {
            std::cout << "\n===============================\n";
            std::cout << "          GAME OVER !          \n";
            std::cout << "        Score final : " << score << "\n";
            std::cout << "===============================\n";
            // Retour au menu principal après un Game Over
            state = GameState::MainMenu;
        }
    }
}

void Game::render() {
    window.clear();

    if (state == GameState::MainMenu) {
        mainMenu->draw(window);
    }
    else if (state == GameState::OptionMenu) {
        optionMenu->draw(window);
    }
    else {
        window.draw(*bg1);
        window.draw(*bg2);

        if (state == GameState::Ready) {
            window.draw(*startButton);
        }
        else if (state == GameState::Playing) {
            for (const auto& obs : obstacles) {
                window.draw(obs.getTopSprite());
                window.draw(obs.getBottomSprite());
            }
            window.draw(player->getSprite());
            window.draw(*scoreText);
        }
    }

    window.display();
}