#include "GameEvent.h"

#include <chrono>

GameEvent::GameEvent(RessourcesManager& rm) :

    rm(rm), spawnTimer(0.f), 
    spawnInterval(1.5f), elapsedTime(0.f),
    baseSpeed(100.f), speedMultiplier(1.f), maxSpeedMultiplier(3.5),

    gapDist(100.f, 350.f), chanceDist(0.f, 100.f) {

    std::random_device rd;
    gen = std::mt19937(rd());
}


void GameEvent::reset(){

    spawnTimer = 0.f;
    elapsedTime = 0.f;
    speedMultiplier = 1.f;
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
    spawnTimer += dt;

    // Augmentation progressive de la vitesse
    speedMultiplier = 1.f + elapsedTime / 60.f;

    // Limite de vitesse
    if (speedMultiplier > maxSpeedMultiplier)
        speedMultiplier = maxSpeedMultiplier;

    // Spawn plus fréquent avec le temps
    float currentInterval = spawnInterval - elapsedTime / 120.f;

    if (currentInterval < 0.6f)
        currentInterval = 0.6f;

    if (spawnTimer >= currentInterval)
    {
        spawnObstacle(obstacles);
        spawnTimer = 0.f;
    }

    // Mise à jour des obstacles avec vitesse augmentée
    for (auto& obs : obstacles)
    {
        obs.update(dt * speedMultiplier);
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

    obstacles.emplace_back(800.f, gap, 150.f, rm, type);
}