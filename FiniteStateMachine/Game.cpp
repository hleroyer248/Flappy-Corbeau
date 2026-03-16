#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode({ 1920, 1080 }), "Flappy Bird - SFML 3.0.2", sf::State::Fullscreen),
state(GameState::MainMenu), score(0), pipeSpawnTimer(0.f), lastPipeWasMoving(false),
player(nullptr) {
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    if (!rm.loadAll()) { std::exit(-1); }
    if (!am.loadAll()) { std::exit(-1); }
    gameEvent.emplace(rm);

    player = new Player(rm);
    save.equipSkin(-1);

    mainMenu.emplace(rm);
    optionMenu.emplace(rm);
    gameOverMenu.emplace(rm);
    shop.emplace(rm, save);

    mainMenu->updateScores(save.getBestScore(), save.getTotalScore());

    backLayers.emplace_back(rm.getBgTexture(), 0.3f);
    backLayers.emplace_back(rm.getMidBgTexture(), 0.6f);
    frontLayers.emplace_back(rm.getFrontBgTexture(), 1.5f);

    menuTitle.emplace(rm.getFont(), "FLAPPY BIRD", 90);
    menuTitle->setPosition({ 700.f, 250.f });
    menuTitle->setFillColor(sf::Color::White);

    startButton.emplace(rm.getFont(), "[ Cliquez ou appuyez sur Espace pour jouer ]", 40);
    startButton->setPosition({ 550.f, 600.f });
    startButton->setFillColor(sf::Color::Yellow);

    scoreText.emplace(rm.getFont(), "Score: 0", 40);
    scoreText->setPosition({ 30.f, 30.f });
    scoreText->setFillColor(sf::Color::White);

    std::random_device rd;
    gen = std::mt19937(rd());

    gapDist = std::uniform_real_distribution<float>(200.f, 650.f);
    chanceDist = std::uniform_real_distribution<float>(0.f, 100.f);
}

void Game::resetGame() {
    player->reset();

    gameEvent->reset();

    obstacles.clear();
    score = 0;
    scoreText->setString("Score: 0");

    int skinIndex = save.getEquippedSkin();

    // Commit Player Default - debut
   /* if (skinIndex == -1) {
        player->setSkin(rm.getPlayerTexture());
    }*/

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

        if (event.is<sf::Event::Closed>()) window.close();

        if (state == GameState::MainMenu) {
            auto action = mainMenu->handleEvent(event);
            if (action == MainMenu::Action::Play) {
                state = GameState::Ready;
                resetGame();
            }
            else if (action == MainMenu::Action::Options) state = GameState::OptionMenu;
            else if (action == MainMenu::Action::Shop) state = GameState::Shop;
            else if (action == MainMenu::Action::Quit) window.close();
        }
        else if (state == GameState::GameOver) {
            auto action = gameOverMenu->handleEvent(event);
            if (action == GameOverMenu::Action::Retry) {
                state = GameState::Ready;
                resetGame();
            }
            else if (action == GameOverMenu::Action::Quit) state = GameState::MainMenu;
        }
        else if (state == GameState::OptionMenu) {
            auto action = optionMenu->handleEvent(event);
            if (action == OptionMenu::Action::Return) state = GameState::MainMenu;
        }
        else if (state == GameState::Shop) {
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    auto action = shop->handleClick(mousePos);
                    if (action == Boutique::Action::Return) state = GameState::MainMenu;
                }
            }
            if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) state = GameState::MainMenu;
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
                am.playMusic();
                am.playJumpSound();
            }
        }
        else if (state == GameState::Playing) {
            if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) {
                    player->flap();
                    am.playJumpSound();
                }
                if ((key->code == sf::Keyboard::Key::LShift || key->code == sf::Keyboard::Key::RShift) && player->canActivateGhost()) {
                    player->activateGhost();
                }
            }
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    player->flap();
                    am.playJumpSound();
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

    float baseSpeed = 200.f;

    for (auto& layer : backLayers) layer.update(dt, baseSpeed);
    for (auto& layer : frontLayers) layer.update(dt, baseSpeed);

    if (state == GameState::Playing) {
        player->update(dt);
        player->updateGhost(dt);

        gameEvent->update(dt, obstacles //, player->getPosition().x, score
        );

        if (gameEvent->shouldClearObstacles)
        {
            obstacles.clear();
            gameEvent->shouldClearObstacles = false;
        }

        if (gameEvent->laserDodgedThisFrame)
        {
            score++;
            scoreText->setString("Score: " + std::to_string(score));
            gameEvent->laserDodgedThisFrame = false;
        }

        //pipeSpawnTimer += dt;
        //if (pipeSpawnTimer > 1.5f) {
        //    ObstacleType spawnType = ObstacleType::Normal;
        //    if (!lastPipeWasMoving && chanceDist(gen) < 20.f) {
        //        if (chanceDist(gen) < 75.f) spawnType = ObstacleType::ParMouv;
        //        else spawnType = ObstacleType::MachMouv;
        //    }
        //    lastPipeWasMoving = (spawnType != ObstacleType::Normal);

        //    float playerSize = 115.f;

        //    // MODIFICATION ICI : On réduit l'écart entre les tuyaux (de 2.0f à 1.6f)
        //    float gapHeight = playerSize * 1.4f;

        //    std::uniform_int_distribution<> pipeDist(0, 2);
        //    int topIdx = pipeDist(gen);
        //    int botIdx = pipeDist(gen);

        //    obstacles.emplace_back(1920.f, gapDist(gen), gapHeight, rm, spawnType, topIdx, botIdx);
        //    pipeSpawnTimer = 0.f;
        //}

        bool collision = false;
        CollisionBox pBox = player->getCollisionBox();
        if (gameEvent->isLaserActive())
        {
            if (pBox.intersects(gameEvent->getLaserCollisionBox()))
            {
                collision = true;
            }
        }
        for (auto it = obstacles.begin(); it != obstacles.end(); ) {
            //it->update(dt);

            if (!player->isGhost()) {
                if (pBox.intersects(it->getTopCollisionBox()) || pBox.intersects(it->getBottomCollisionBox())) {
                    collision = true;
                }
            }

            if (!it->isPassed() && it->getX() + it->getWidth() < player->getPosition().x - (pBox.getRect().size.x / 2.f)) {
                it->setPassed(true);
                score++;
                scoreText->setString("Score: " + std::to_string(score));
                gameEvent->addObstaclePassed(obstacles);
            }

            if (it->getX() + it->getWidth() < 0.f) it = obstacles.erase(it);
            else ++it;
        }

        if (!player->isGhost()) {
            if (pBox.getRect().position.y < 0.f || pBox.getRect().position.y + pBox.getRect().size.y > 1080.f) {
                collision = true;
            }
        }

        if (collision) {
            am.stopMusic();
            am.playDeathSound();
            state = GameState::GameOver;
            gameOverMenu->updateScoreText(score);
            save.addScore(score);
            gameOverMenu->updateBestScore(save.getBestScore());
            mainMenu->updateScores(save.getBestScore(), save.getTotalScore());
        }
    }

    if (state == GameState::OptionMenu) {

        optionMenu->update(window);

        float volume = optionMenu->getVolume();
        am.setVolume(volume);

        return;
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
        for (const auto& layer : backLayers) layer.draw(window);

        // 

        for (const auto& layer : frontLayers) {
            layer.draw(window);
        }

        if (state == GameState::Playing || state == GameState::Ready || state == GameState::GameOver) {
            for (const auto& obs : obstacles) {
                window.draw(obs.getTopSprite());
                window.draw(obs.getBottomSprite());
            }
            window.draw(player->getSprite());
            gameEvent->draw(window);

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