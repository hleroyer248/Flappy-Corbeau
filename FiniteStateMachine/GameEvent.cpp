#include "GameEvent.h"
#include <iostream>

GameEvent::GameEvent(RessourcesManager& rm) :
    rm(rm),
    spawnTimer(0.f),
    spawnInterval(1.5f),
    elapsedTime(0.f),
    baseSpeed(100.f),
    speedMultiplier(1.f),
    maxSpeedMultiplier(4.0),
    gapDist(150.f, 550.f),
    chanceDist(0.f, 100.f),
    laserYDist(100.f, 800.f),
    state(EventState::Normal),
    obstaclesPassed(0),
    lasersDodged(0),
    warningTimer(0.f),
    laserTimer(0.f),
    laserAnimTimer(0.f),
    currentLaserFrame(0),
    laserSprites{ sf::Sprite(rm.getLaserTexture()), sf::Sprite(rm.getLaserTexture()) } {

    for (int i = 0; i < 2; ++i) {
        // CORRECTION ICI : Le rectangle fait exactement 60 pixels (la taille du laser) au lieu de 100
        warningRects[i].setSize({ 1920.f, 60.f });
        warningRects[i].setFillColor(sf::Color(255, 0, 0, 150));

        sf::IntRect firstRect = rm.getLaserRect(0);
        if (firstRect.size.x > 0) {
            float scaleX = 1920.f / static_cast<float>(firstRect.size.x);
            float scaleY = 60.f / static_cast<float>(firstRect.size.y);
            laserSprites[i].setTextureRect(firstRect);
            laserSprites[i].setScale({ scaleX, scaleY });
        }
    }

    std::random_device rd;
    gen = std::mt19937(rd());

    laserDodgedThisFrame = false;
}

void GameEvent::reset() {
    spawnTimer = 0.f;
    elapsedTime = 0.f;
    speedMultiplier = 1.f;
    state = EventState::Normal;
    obstaclesPassed = 0;
    lasersDodged = 0;
    warningTimer = 0.f;
    laserTimer = 0.f;
    shouldClearObstacles = false;
}

void GameEvent::update(float dt, std::vector<Obstacle>& obstacles) {
    elapsedTime += dt;

    speedMultiplier = std::min(1.f + elapsedTime / 300.f, maxSpeedMultiplier);
    float currentInterval = std::max(spawnInterval - elapsedTime / 720.f, 0.6f);

    if (state == EventState::Normal) {
        spawnTimer += dt;
        if (spawnTimer >= currentInterval) {
            spawnObstacle(obstacles);
            spawnTimer = 0.f;
        }
        for (auto& obs : obstacles) {
            obs.update(dt * speedMultiplier);
        }
    }
    else if (state == EventState::Warning) {
        warningTimer += dt;
        if (warningTimer >= 1.5f) {
            state = EventState::Laser;
            laserTimer = 0.f;
            currentLaserFrame = 0;
            laserAnimTimer = 0.f;

            sf::IntRect currentRect = rm.getLaserRect(0);
            if (currentRect.size.x > 0) {
                float desiredThickness = 60.f;
                float scaleX = 1920.f / static_cast<float>(currentRect.size.x);
                float scaleY = desiredThickness / static_cast<float>(currentRect.size.y);

                for (int i = 0; i < 2; ++i) {
                    laserSprites[i].setTextureRect(currentRect);
                    laserSprites[i].setScale({ scaleX, scaleY });

                    // CORRECTION ICI : Plus de yOffset, on place le laser exactement à la même hauteur que le rectangle rouge
                    laserSprites[i].setPosition({ 0.f, warningRects[i].getPosition().y });
                }
            }
        }
    }
    else if (state == EventState::Laser) {
        // --- ANIMATION ET SCALING ---
        laserAnimTimer += dt;

        float frameDuration = 0.12f;

        if (laserAnimTimer >= frameDuration) {
            laserAnimTimer -= frameDuration;
            currentLaserFrame = (currentLaserFrame + 1) % rm.getLaserFrameCount();
        }

        sf::IntRect currentRect = rm.getLaserRect(currentLaserFrame);
        if (currentRect.size.x > 0) {
            float desiredThickness = 60.f;
            float scaleX = 1920.f / static_cast<float>(currentRect.size.x);
            float scaleY = desiredThickness / static_cast<float>(currentRect.size.y);

            for (int i = 0; i < 2; ++i) {
                laserSprites[i].setTextureRect(currentRect);
                laserSprites[i].setScale({ scaleX, scaleY });

                // CORRECTION ICI AUSSI : On place le laser à la position exacte du Warning
                laserSprites[i].setPosition({ 0.f, warningRects[i].getPosition().y });
            }
        }

        // --- GESTION DU TEMPS ---
        laserTimer += dt;
        if (laserTimer >= 2.5f) {
            lasersDodged++;
            laserDodgedThisFrame = true;

            if (lasersDodged >= 5) {
                lasersDodged = 0;
                state = EventState::Normal;
            }
            else {
                float newY1, newY2;
                do {
                    newY1 = laserYDist(gen);
                    newY2 = laserYDist(gen);
                } while (std::abs(newY1 - newY2) < 300.f);

                warningRects[0].setPosition({ 0.f, newY1 });
                warningRects[1].setPosition({ 0.f, newY2 });

                state = EventState::Warning;
                warningTimer = 0.f;
            }
        }
    }
}

void GameEvent::addObstaclePassed(std::vector<Obstacle>& obstacles) {
    obstaclesPassed++;

    if (obstaclesPassed >= 10 && state == EventState::Normal) {
        state = EventState::Warning;
        warningTimer = 0.f;
        obstaclesPassed = 0;
        shouldClearObstacles = true;

        float newY1, newY2;
        do {
            newY1 = laserYDist(gen);
            newY2 = laserYDist(gen);
        } while (std::abs(newY1 - newY2) < 300.f);

        warningRects[0].setPosition({ 0.f, newY1 });
        warningRects[1].setPosition({ 0.f, newY2 });
    }
}

void GameEvent::draw(sf::RenderWindow& window) {
    if (state == EventState::Warning) {
        window.draw(warningRects[0]);
        window.draw(warningRects[1]);
    }
    if (state == EventState::Laser) {
        window.draw(laserSprites[0]);
        window.draw(laserSprites[1]);
    }
}

std::vector<CollisionBox> GameEvent::getLaserCollisionBoxes() const {
    std::vector<CollisionBox> boxes;
    boxes.push_back(CollisionBox(laserSprites[0].getGlobalBounds()));
    boxes.push_back(CollisionBox(laserSprites[1].getGlobalBounds()));
    return boxes;
}

bool GameEvent::isLaserActive() const {
    return state == EventState::Laser;
}

void GameEvent::spawnObstacle(std::vector<Obstacle>& obstacles) {
    ObstacleType type = ObstacleType::Normal;

    float specialChance = std::min(15.f + elapsedTime / 120.f, 50.f);
    float roll = chanceDist(gen);

    if (roll < specialChance) {
        type = (roll < specialChance / 2.f) ? ObstacleType::ParMouv : ObstacleType::MachMouv;
    }

    std::uniform_int_distribution<int> pipeDist(0, 2);
    obstacles.emplace_back(1950.f, gapDist(gen), 210.f, rm, type, pipeDist(gen), pipeDist(gen));
}
