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
    gapDist(250.f, 700.f),
    chanceDist(0.f, 100.f),
    laserYDist(100.f, 800.f), // Ajusté pour éviter que le laser sorte trop de l'écran
    state(EventState::Normal),
    obstaclesPassed(0),
    lasersDodged(0),
    warningTimer(0.f),
    laserTimer(0.f),
    laserSprite(rm.getLaserTexture()) {

    laserSprite.setTexture(rm.getLaserTexture(), true);

    float screenWidth = 1920.f;
    float textureWidth = static_cast<float>(rm.getLaserTexture().getSize().x);
    laserSprite.setScale({ screenWidth / textureWidth, 1.f });

    std::random_device rd;
    gen = std::mt19937(rd());

    // Configuration initiale du rectangle d'alerte
    warningRect.setSize({ 1920.f, 100.f });
    warningRect.setFillColor(sf::Color(255, 0, 0, 150));

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

    // MODIFICATION 1 : Accélération divisée par 6 (50.f * 6 = 300.f)
    speedMultiplier = std::min(1.f + elapsedTime / 300.f, maxSpeedMultiplier);

    // L'intervalle de spawn est aussi ralenti pour correspondre (120.f * 6 = 720.f)
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
        }
    }
    else if (state == EventState::Laser) {
        laserTimer += dt;
        if (laserTimer >= 2.5f) {
            lasersDodged++;
            laserDodgedThisFrame = true;

            // MODIFICATION 2 : Le nombre de lasers à esquiver passe de 10 à 5
            if (lasersDodged >= 5) {
                lasersDodged = 0;
                state = EventState::Normal;
            }
            else {
                float oldY = laserSprite.getPosition().y;
                float newY;
                float minDistance = 250.f;

                do {
                    newY = laserYDist(gen);
                } while (std::abs(newY - oldY) < minDistance);

                warningRect.setPosition({ 0.f, newY });
                laserSprite.setPosition({ 0.f, newY });

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

        float oldY = laserSprite.getPosition().y;
        float newY;

        do {
            newY = laserYDist(gen);
        } while (std::abs(newY - oldY) < 250.f);

        warningRect.setPosition({ 0.f, newY });
        laserSprite.setPosition({ 0.f, newY });
    }
}

void GameEvent::draw(sf::RenderWindow& window) {
    if (state == EventState::Warning) window.draw(warningRect);
    if (state == EventState::Laser) window.draw(laserSprite);
}

CollisionBox GameEvent::getLaserCollisionBox() const {
    return CollisionBox(laserSprite.getGlobalBounds());
}

bool GameEvent::isLaserActive() const {
    return state == EventState::Laser;
}

void GameEvent::spawnObstacle(std::vector<Obstacle>& obstacles) {
    ObstacleType type = ObstacleType::Normal;

    // On ralentit aussi l'apparition des tuyaux spéciaux (20.f * 6 = 120.f)
    float specialChance = std::min(15.f + elapsedTime / 120.f, 50.f);
    float roll = chanceDist(gen);

    if (roll < specialChance) {
        type = (roll < specialChance / 2.f) ? ObstacleType::ParMouv : ObstacleType::MachMouv;
    }

    std::uniform_int_distribution<int> pipeDist(0, 2);
    obstacles.emplace_back(1950.f, gapDist(gen), 60.f, rm, type, pipeDist(gen), pipeDist(gen));
}
