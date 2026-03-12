#include "GameEvent.h"

#include <chrono>

GameEvent::GameEvent(RessourcesManager& rm) :

    rm(rm), spawnTimer(0.f), 
    spawnInterval(1.5f), elapsedTime(0.f),
    baseSpeed(100.f), speedMultiplier(1.f),

    gapDist(100.f, 350.f), chanceDist(0.f, 100.f) {

    std::random_device rd;
    gen = std::mt19937(rd());
}

void GameEvent::update(float dt, std::vector<Obstacle>& obstacles, float playerX, int score) {

    elapsedTime += dt;
    spawnTimer += dt;

    // Augmente la vitesse progressivement
    speedMultiplier = 1.f + elapsedTime / 60.f; // +1x toutes les 60s

    // Diminue progressivement l'intervalle d'apparition
    float currentInterval = spawnInterval - elapsedTime / 120.f; // min ~0.5s
    if (currentInterval < 0.5f) currentInterval = 0.5f;

    if (spawnTimer >= currentInterval) {
        spawnObstacle(obstacles);
        spawnTimer = 0.f;
    }

    // Mise à jour des obstacles
    for (auto& obs : obstacles) {
        obs.update(dt * speedMultiplier);
    }
}

void GameEvent::spawnObstacle(std::vector<Obstacle>& obstacles) {

    ObstacleType type = ObstacleType::Normal;

    // Chance augmentée pour les obstacles spéciaux
    float specialChance = 15.f + elapsedTime / 30.f; // +1% toutes les 30s
    if (specialChance > 60.f) specialChance = 60.f;

    float roll = chanceDist(gen);
    if (roll < specialChance) {
        type = (roll < specialChance / 2.f) ? ObstacleType::ParMouv : ObstacleType::MachMouv;
    }

    float gap = gapDist(gen);
    obstacles.emplace_back(800.f, gap, 150.f, rm, type);
}