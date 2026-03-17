#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode({ 1920, 1080 }), "Flappy Bird - SFML 3.0.2", sf::State::Windowed),
state(GameState::MainMenu), score(0), pipeSpawnTimer(0.f), lastPipeWasMoving(false),
player(nullptr), capaIcon(rm.getCapaTexture())
{
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

    backLayers.emplace_back(rm.getBgTexture(), 0.3f);
    backLayers.emplace_back(rm.getMidBgTexture(), 0.6f);
    frontLayers.emplace_back(rm.getFrontBgTexture(), 1.5f);

    menuTitle.emplace(rm.getFont(), "FLAPPY BIRD", 90);
    menuTitle->setPosition({ 700.f, 250.f });
    menuTitle->setFillColor(sf::Color::White);

    startButton.emplace(rm.getFont(), "[ Cliquez ou appuyez sur Espace pour jouer ]", 40);
    startButton->setPosition({ 550.f, 600.f });
    startButton->setFillColor(sf::Color::Yellow);

    std::random_device rd;
    gen = std::mt19937(rd());

    gapDist = std::uniform_real_distribution<float>(200.f, 650.f);
    chanceDist = std::uniform_real_distribution<float>(0.f, 100.f);

    capaIcon.setTexture(rm.getCapaTexture());

    // 🔥 TESTS
    capaIcon.setTextureRect(sf::IntRect({ 0,0 }, {
        (int)rm.getCapaTexture().getSize().x,
        (int)rm.getCapaTexture().getSize().y
        }));

    capaIcon.setScale({ 0.05f, 0.05f });
    capaIcon.setPosition({ 100.f, 800.f });

    cooldownArc = sf::VertexArray(sf::PrimitiveType::TriangleFan);
}

void Game::resetGame() {
    player->reset();
    gameEvent->reset();
    obstacles.clear();
    score = 0;

    int skinIndex = save.getEquippedSkin();

    if (skinIndex == 0) player->setSkin(rm.getPlayerTexture());
    if (skinIndex == 1) player->setSkin(rm.getBirdRedTexture());
    if (skinIndex == 2) player->setSkin(rm.getBirdBlueTexture());
    if (skinIndex == 3) player->setSkin(rm.getBirdGreenTexture());
    if (skinIndex == 4) player->setSkin(rm.getBirdGoldTexture());
    if (skinIndex == 5) player->setSkin(rm.getBirdShadowTexture());

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

        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::F1) {
                debugMode = !debugMode; // Toggle le mode debug
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
        float ratio = player->getCooldownRatio();
        float angleMax = 360.f * ratio;

        cooldownArc.clear();
        sf::FloatRect bounds = capaIcon.getGlobalBounds();

        sf::Vector2f center(
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        );
        center.x += 5.f;
        cooldownArc.append(sf::Vertex(center, sf::Color(100, 100, 100, 150)));

        for (int i = 0; i <= 100; i++) {
            float angle = (-90.f + (angleMax * i / 100.f)) * 3.14159f / 180.f;

            float x = center.x + cos(angle) * 48.f;
            float y = center.y + sin(angle) * 48.f;

            cooldownArc.append(sf::Vertex({ x, y }, sf::Color(100, 100, 100, 150)));
        }

        gameEvent->update(dt, obstacles);

        if (gameEvent->shouldClearObstacles)
        {
            obstacles.clear();
            gameEvent->shouldClearObstacles = false;
        }

        if (gameEvent->laserDodgedThisFrame)
        {
            score++;
            gameEvent->laserDodgedThisFrame = false;
        }

        bool collision = false;
        CollisionBox pBox = player->getCollisionBox();

        if (gameEvent->isLaserActive() && !player->isGhost()) {
            if (pBox.intersects(gameEvent->getLaserCollisionBox())) {
                collision = true;
            }
        }

        for (auto it = obstacles.begin(); it != obstacles.end(); ) {

            if (!player->isGhost()) {
                if (pBox.intersects(it->getTopCollisionBox()) || pBox.intersects(it->getBottomCollisionBox())) {
                    collision = true;
                }
            }

            if (!it->isPassed() && it->getX() + it->getWidth() < player->getPosition().x - (pBox.getRect().size.x / 2.f)) {
                it->setPassed(true);
                score++;
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
        }
    }
}

void Game::drawScore(sf::RenderWindow& window, int scoreVal, sf::Vector2f position, float scale, bool center) {
    const sf::Texture& tex = rm.getNumbersTexture();
    if (tex.getSize().x == 0) return;

    std::string s = std::to_string(scoreVal);

    // Espacement dynamique pour le score in-game
    float spacing = 20.f * scale;
    float totalWidth = 0;

    for (char c : s) {
        int d = c - '0';
        totalWidth += rm.getDigitRect(d).size.x * scale + spacing;
    }
    totalWidth -= spacing;

    float currentX = position.x;
    if (center) {
        currentX -= totalWidth / 2.f;
    }

    sf::Sprite spr(tex);
    spr.setScale({ scale, scale });

    for (char c : s) {
        int d = c - '0';
        sf::IntRect rect = rm.getDigitRect(d);

        spr.setTextureRect(rect);
        spr.setPosition({ currentX, position.y });
        window.draw(spr);

        currentX += rect.size.x * scale + spacing;
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
    else if (state == GameState::Shop) {
        shop->draw(window);
    }
    else {
        for (const auto& layer : backLayers) layer.draw(window);

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

            // MODIFICATION DE L'ÉCHELLE : On passe de 1.0f à 0.35f
            drawScore(window, score, { 30.f, 5.f }, 0.12f, false);
        }
        else if (state == GameState::Playing) {
            // MODIFICATION DE L'ÉCHELLE : On passe de 1.0f à 0.35f
            drawScore(window, score, { 30.f, 5.f }, 0.12f, false);
            window.draw(capaIcon);

            if (!player->canActivateGhost()) {
                window.draw(cooldownArc);
            }

        }
        if (state == GameState::GameOver) {
            gameOverMenu->draw(window);
        }
    }

    if (debugMode) {
        // 1. Hitbox du Joueur
        player->getCollisionBox().debugDraw(window);

        // 2. Hitbox des Obstacles
        for (const auto& obs : obstacles) {
            obs.getTopCollisionBox().debugDraw(window);
            obs.getBottomCollisionBox().debugDraw(window);
        }

        // 3. Hitbox du Laser
        if (gameEvent->isLaserActive()) {
            gameEvent->getLaserCollisionBox().debugDraw(window);
        }
    }
    window.display();
}