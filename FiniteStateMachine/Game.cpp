#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode({ 1920, 1080 }), "RavenSoul", sf::State::Windowed),
state(GameState::MainMenu), score(0), pipeSpawnTimer(0.f), lastPipeWasMoving(false),
player(nullptr), capaIcon(rm.getCapaTexture())
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    if (!rm.loadAll()) { std::exit(-1); }
    if (!am.loadAll()) { std::exit(-1); }
    gameEvent.emplace(rm, am);

    frontLayers.clear();

    // layer (sol)
    frontLayers.emplace_back(rm.getFrontBottomTexture(), 1.2f);

    // layer (plafond)
    frontLayers.emplace_back(rm.getFrontTopTexture(), 1.2f);

    player = new Player(rm);
    save.equipSkin(-1);

    mainMenu.emplace(rm);
    optionMenu.emplace(rm, am);
    gameOverMenu.emplace(rm);
    shop.emplace(rm, save);

    backLayers.clear();
    backLayers.emplace_back(rm.getBackBgTexture(), 0.3f);
    backLayers.emplace_back(rm.getMidBgTexture(), 0.6f);

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

    player->setSkin(rm.getPlayerTexture());

    sf::Color skinColor = sf::Color::White; 

    switch (skinIndex)
    {
    case 0: skinColor = sf::Color::White; break;
    case 1: skinColor = sf::Color::Red; break;
    case 2: skinColor = sf::Color::Blue; break;
    case 3: skinColor = sf::Color::Green; break;
    case 4: skinColor = sf::Color(255, 215, 0); break;
    case 5: skinColor = sf::Color(100, 100, 100); break;
    case 6:
        break;
    }

    if (skinIndex != 6) {
        player->setSkinColor(skinColor);
    }

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
                    am.playGhostSound();
                }
            }
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    player->flap();
                }
            }
        }

        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::F1) {
                debugMode = !debugMode;
            }
        }
    }
}

void Game::update(float dt) {
    am.updateMusic();
    am.updateCrow(dt);
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

        if (save.getEquippedSkin() == 6) {
            float time = rainbowClock.getElapsedTime().asSeconds();
            player->setSkinColor(getRainbowColor(time));
        }
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
            for (const auto& box : gameEvent->getLaserCollisionBoxes()) {
                if (pBox.intersects(box)) {
                    collision = true;
                    break;
                }
            }
        }

        if (!player->isGhost()) {
            for (auto it = obstacles.begin(); it != obstacles.end(); ) {
                if (pBox.intersects(it->getTopCollisionBox()) || pBox.intersects(it->getBottomCollisionBox())) {
                    collision = true;
                }

                if (!it->isPassed() && it->getX() + it->getWidth() < player->getPosition().x - (pBox.getRect().size.x / 2.f)) {
                    it->setPassed(true);
                    score++;
                    gameEvent->addObstaclePassed(obstacles);
                }

                if (it->getX() + it->getWidth() < 0.f) it = obstacles.erase(it);
                else ++it;
            }
        }

        else {
            for (auto& obs : obstacles) {
                if (!obs.isPassed() && obs.getX() + obs.getWidth() < player->getPosition().x - (pBox.getRect().size.x / 2.f)) {
                    obs.setPassed(true);
                    score++;
                    gameEvent->addObstaclePassed(obstacles);
                }
            }
        }

        if (pBox.getRect().position.y < 0.f || pBox.getRect().position.y + pBox.getRect().size.y > 1080.f) {
            collision = true;
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

        if (state == GameState::Playing || state == GameState::Ready || state == GameState::GameOver) {
            for (const auto& obs : obstacles) {
                window.draw(obs.getTopSprite());

                const sf::Sprite& body = obs.getBottomBody();
                float y = body.getPosition().y;
                float height = body.getGlobalBounds().size.y - 12.f;
                bool first = true;

                while (y < 1100.f) {
                    sf::Sprite part = body;
                    if (!first) {
                        sf::IntRect rect = part.getTextureRect();
                        int cut = 149;
                        rect.position.y += cut;
                        rect.size.y -= cut;
                        part.setTextureRect(rect);
                    }
                    part.setPosition({ body.getPosition().x, y });
                    window.draw(part);
                    y += height;
                    first = false;
                }
                window.draw(obs.getBottomHead());
            }

            window.draw(player->getSprite());
            gameEvent->draw(window);
        }

        for (const auto& layer : frontLayers) {
            layer.draw(window);
        }

        if (state == GameState::Ready) {
            window.draw(*startButton);
            drawScore(window, score, { 30.f, 5.f }, 0.12f, false);
        }
        else if (state == GameState::Playing) {
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

    if (debugMode && (state == GameState::Playing || state == GameState::Ready)) {
        player->getCollisionBox().debugDraw(window);
        for (const auto& obs : obstacles) {
            obs.getTopCollisionBox().debugDraw(window);
            obs.getBottomCollisionBox().debugDraw(window);
        }
    }

    if (gameEvent->isLaserActive()) {
        for (const auto& box : gameEvent->getLaserCollisionBoxes()) {
            box.debugDraw(window);
        }
    }

    window.display();
}
