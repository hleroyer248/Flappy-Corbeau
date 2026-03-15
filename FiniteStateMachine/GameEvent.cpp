#include "GameEvent.h"
#include <iostream>
#include <chrono>

GameEvent::GameEvent(RessourcesManager& rm) :

    rm(rm), 
    spawnTimer(0.f), 
    spawnInterval(1.5f),
    elapsedTime(0.f),
    baseSpeed(100.f), 
    speedMultiplier(1.f), 
    maxSpeedMultiplier(4.0),

    gapDist(100.f, 350.f),
    chanceDist(0.f, 100.f),
    state(EventState::Normal),
    obstaclesPassed(0),
    lasersDodged(0),
    warningTimer(0.f),
    laserTimer(0.f),
    laserSprite(rm.getLaserTexture()){

    laserSprite.setTexture(rm.getLaserTexture(), true);

    std::random_device rd;
    gen = std::mt19937(rd());

    // WARNING RECTANGLE
    warningRect.setSize({ 1920.f, 100.f });
    warningRect.setFillColor(sf::Color(255, 0, 0, 150));
    warningRect.setPosition({ 0.f, 500.f });

    // LASER SPRITE
    laserSprite.setPosition({ 300.f, 500.f });
    laserDodgedThisFrame = false;
    laserSprite.setScale({ 2.f, 2.f });
}


void GameEvent::reset()
{
    spawnTimer = 0.f;
    elapsedTime = 0.f;
    speedMultiplier = 1.f;

    state = EventState::Normal;
    obstaclesPassed = 0;
    lasersDodged = 0;

    warningTimer = 0.f;
    laserTimer = 0.f;
}

/*
void GameEvent::update(float dt, std::vector<Obstacle>& obstacles, float playerX, int score) {

    elapsedTime += dt;
    spawnTimer += dt;

    // Augmente la vitesse progressivement
    speedMultiplier = 1.f + elapsedTime / 30.f; // +1x toutes les 60s

    // Diminue progressivement l'intervalle d'apparition
    float currentInterval = spawnInterval - elapsedTime / 80.f; // 120s
    if (currentInterval < 0.5f) currentInterval = 0.5f;

    if (spawnTimer >= currentInterval) {
        spawnObstacle(obstacles);
        spawnTimer = 0.f;
    }

    // Mise à jour des obstacles
    for (auto& obs : obstacles) {
        obs.update(dt * speedMultiplier);
    }
} */


void GameEvent::update(float dt, std::vector<Obstacle>& obstacles)
{
    elapsedTime += dt;

    speedMultiplier = 1.f + elapsedTime / 50.f;

    if (speedMultiplier > maxSpeedMultiplier)
        speedMultiplier = maxSpeedMultiplier;

    float currentInterval = spawnInterval - elapsedTime / 120.f;

    if (currentInterval < 0.6f)
        currentInterval = 0.6f;

    if (state == EventState::Normal)
    {
        spawnTimer += dt;

        if (spawnTimer >= currentInterval)
        {
            spawnObstacle(obstacles);
            spawnTimer = 0.f;
        }

        for (auto& obs : obstacles)
        {
            obs.update(dt * speedMultiplier);
        }
    }

    else if (state == EventState::Warning)
    {
        warningTimer += dt;

        if (warningTimer >= 1.5f)
        {
            state = EventState::Laser;
            laserTimer = 0.f;
        }
    }

    else if (state == EventState::Laser)
    {
        laserTimer += dt;

        if (laserTimer >= 2.5f)
        {
            lasersDodged++;

            laserDodgedThisFrame = true; 

            if (lasersDodged >= 10)
            {
                lasersDodged = 0;
                state = EventState::Normal;
            }
            else
            {
                state = EventState::Warning;
                warningTimer = 0.f;
            }
        }
    }
}

/*void GameEvent::spawnObstacle(std::vector<Obstacle>& obstacles) {

    ObstacleType type = ObstacleType::Normal;

    // Chance augmentée pour les obstacles spéciaux
    float specialChance = 15.f + elapsedTime / 15.f; // +1% toutes les 30s
    if (specialChance > 60.f) specialChance = 60.f;

    float roll = chanceDist(gen);
    if (roll < specialChance) {
        type = (roll < specialChance / 2.f) ? ObstacleType::ParMouv : ObstacleType::MachMouv;
    }

    float gap = gapDist(gen);
    obstacles.emplace_back(800.f, gap, 150.f, rm, type);
}
*/

void GameEvent::spawnObstacle(std::vector<Obstacle>& obstacles)
{
    ObstacleType type = ObstacleType::Normal;

    float specialChance = 15.f + elapsedTime / 20.f;

    if (specialChance > 50.f)
        specialChance = 50.f;

    float roll = chanceDist(gen);

    if (roll < specialChance)
    {
        if (roll < specialChance / 2.f)
            type = ObstacleType::ParMouv;
        else
            type = ObstacleType::MachMouv;
    }

    float gap = gapDist(gen);

    // Choix aléatoire du style de tuyau
    std::uniform_int_distribution<int> pipeDist(0, 2);

    int topIdx = pipeDist(gen);
    int botIdx = pipeDist(gen);

    obstacles.emplace_back(
        1950.f,
        gap,
        150.f,
        rm,
        type,
        topIdx,
        botIdx
    );
}

void GameEvent::addObstaclePassed(std::vector<Obstacle>& obstacles)
{
    obstaclesPassed++;

    if (obstaclesPassed >= 10 && state == EventState::Normal)
    {
        state = EventState::Warning;
        warningTimer = 0.f;
        obstaclesPassed = 0;

        shouldClearObstacles = true;
    }
}

void GameEvent::draw(sf::RenderWindow& window)
{
    if (state == EventState::Warning)
    {
        window.draw(warningRect);
    }

    if (state == EventState::Laser)
    {
        window.draw(laserSprite);
    }
}