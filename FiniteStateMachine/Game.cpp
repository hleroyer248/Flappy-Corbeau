#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode({ 800, 600 }), "Flappy Bird - SFML 3.0.2"),
state(GameState::MainMenu), score(0), pipeSpawnTimer(0.f), lastPipeWasMoving(false),
player(nullptr) {
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    if (!rm.loadAll()) {
        std::exit(-1);
    }

    // Commit Add Sound - debut
    if (!am.loadAll()) {
        std::exit(-1);
    }
    // Commit Add Sound - fin

    player = new Player(rm);

    // Commit Player Default - debut
    save.equipSkin(-1);
    // Commit Player Default - fin

    mainMenu.emplace(rm);
    optionMenu.emplace(rm);
    gameOverMenu.emplace(rm);
    shop.emplace(rm, save);

    // commit sauvegarde
    mainMenu->updateScores(save.getBestScore(), save.getTotalScore());

    //commit Parallax
    backLayers.emplace_back(rm.getBgTexture(), 0.3f);
    backLayers.emplace_back(rm.getMidBgTexture(), 0.6f);
    frontLayers.emplace_back(rm.getFrontBgTexture(), 1.5f);

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
    int skinIndex = save.getEquippedSkin();
  
    if (skinIndex == 0)
        player->setSkin(rm.getPlayerTexture());

    if (skinIndex == 1)
        player->setSkin(rm.getBirdRedTexture());

    if (skinIndex == 2)
        player->setSkin(rm.getBirdBlueTexture());

    if (skinIndex == 3)
        player->setSkin(rm.getBirdGreenTexture());

    if (skinIndex == 4)
        player->setSkin(rm.getBirdGoldTexture());

    if (skinIndex == 5)
        player->setSkin(rm.getBirdShadowTexture());

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
            else if (action == MainMenu::Action::Shop)
            {
                state = GameState::Shop;
            }
            else if (action == MainMenu::Action::Quit) {
                window.close();
            }
        }

        else if (state == GameState::GameOver) {
            auto action = gameOverMenu->handleEvent(event);
            if (action == GameOverMenu::Action::Retry) {
                state = GameState::Ready;
                resetGame();
            }
            else if (action == GameOverMenu::Action::Quit) {
                state = GameState::MainMenu;
            }
        }

        else if (state == GameState::OptionMenu) {
            auto action = optionMenu->handleEvent(event);
            if (action == OptionMenu::Action::Return) {
                state = GameState::MainMenu;
            }
        }

        else if (state == GameState::Shop) {
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    auto action = shop->handleClick(mousePos);
                    if (action == Boutique::Action::Return) {
                        state = GameState::MainMenu;
                    }
                }
            }

            if (const auto* key = event.getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    state = GameState::MainMenu;
                }
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

                // Commit Add Sound - debut
                am.playMusic();     // Lance la musique
                am.playJumpSound(); // Le premier saut produit aussi un son !
                // Commit Add Sound - fin
            }
        }

        else if (state == GameState::Playing) {
            if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) {
                    player->flap();
                    // Commit Add Sound - debut
                    am.playJumpSound();
                    // Commit Add Sound - fin
                }

                if ((key->code == sf::Keyboard::Key::LShift || key->code == sf::Keyboard::Key::RShift) && player->canActivateGhost()) {
                    player->activateGhost();
                }
            }
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    player->flap();
                    // Commit Add Sound - debut
                    am.playJumpSound();
                    // Commit Add Sound - fin
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (state == GameState::MainMenu || state == GameState::OptionMenu) {
        return;
    }
    if (state == GameState::Shop)
    {
        shop->update(window);
        shop->updateCursor(window);
        return;
    }

    float baseSpeed = 100.f;

    // Mise à jour de tous les plans de fond
    for (auto& layer : backLayers) {
        layer.update(dt, baseSpeed);
    }

    // Mise à jour des plans de devant
    for (auto& layer : frontLayers) {
        layer.update(dt, baseSpeed);
    }

    if (state == GameState::Playing) {
        player->update(dt);
        player->updateGhost(dt);

        pipeSpawnTimer += dt;
        if (pipeSpawnTimer > 1.5f) {
            ObstacleType spawnType = ObstacleType::Normal;

            if (!lastPipeWasMoving && chanceDist(gen) < 20.f) {
                if (chanceDist(gen) < 75.f) {
                    spawnType = ObstacleType::ParMouv;
                }
                else {
                    spawnType = ObstacleType::MachMouv;
                }
            }
            lastPipeWasMoving = (spawnType != ObstacleType::Normal);
            obstacles.emplace_back(800.f, gapDist(gen), 150.f, rm, spawnType);
            pipeSpawnTimer = 0.f;
        }

        bool collision = false;
        CollisionBox pBox = player->getCollisionBox();
        for (auto it = obstacles.begin(); it != obstacles.end(); ) {
            it->update(dt);

            if (!player->isGhost()) {
                if (pBox.intersects(it->getTopCollisionBox()) || pBox.intersects(it->getBottomCollisionBox())) {
                    collision = true;
                }
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

        if (!player->isGhost()) {
            if (pBox.getRect().position.y < 0.f || pBox.getRect().position.y + pBox.getRect().size.y > 600.f) {
                collision = true;
            }
        }

        if (collision) {
            std::cout << "\n===============================\n";
            std::cout << "          GAME OVER !          \n";
            std::cout << "        Score final : " << score << "\n";
            std::cout << "===============================\n";

            // Commit Add Sound - debut
            am.stopMusic();       // Arr�te la musique de fond
            am.playDeathSound();  // Joue le son de d�faite
            // Commit Add Sound - fin

            state = GameState::GameOver;
            gameOverMenu->updateScoreText(score);

            save.addScore(score);
            gameOverMenu->updateBestScore(save.getBestScore());
            mainMenu->updateScores(save.getBestScore(), save.getTotalScore());
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
   /* else if (state == GameState::GameOver) {
        gameOverMenu->draw(window);
    }*/
    else if (state == GameState::Shop) {
        shop->draw(window);
    }
    // défini l'ordre pour dessiner les plan un à un
    else {
        for (const auto& layer : backLayers) {
            layer.draw(window);
        }

        if (state == GameState::Playing || state == GameState::Ready || state == GameState::GameOver) {
            for (const auto& obs : obstacles) {
                window.draw(obs.getTopSprite());
                window.draw(obs.getBottomSprite());
            }
            window.draw(player->getSprite());
        }

        for (const auto& layer : frontLayers) {
            layer.draw(window);
        }

        if (state == GameState::Ready) {
            window.draw(*startButton);
        }
        else if (state == GameState::Playing) {
            window.draw(*scoreText);
        }
        if (state == GameState::GameOver) {
            gameOverMenu->draw(window);
        }
    }

    window.display();
}