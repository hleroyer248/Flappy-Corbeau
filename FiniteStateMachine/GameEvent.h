#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include <array>

#include "RessourcesManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Obstacle.h"
#include "MainMenu.h"
#include "Boutique.h"
#include "OptionMenu.h"

class GameEvent {

public:

    enum class EventState { Normal, Warning, Laser };

    EventState state;

    bool shouldClearObstacles = false;
    bool laserDodgedThisFrame = false;

    // Gère désormais 2 boîtes de collision
    std::vector<CollisionBox> getLaserCollisionBoxes() const;

    int obstaclesPassed;
    int lasersDodged;

    float warningTimer;
    float laserTimer;

    // NOUVEAU : Timer d'animation
    float laserAnimTimer;
    int currentLaserFrame;

    void addObstaclePassed(std::vector<Obstacle>& obstacles);

    // Gère 2 Lasers
    std::array<sf::RectangleShape, 2> warningRects;
    std::array<sf::Sprite, 2> laserSprites;

    bool isLaserActive() const;

    GameEvent(RessourcesManager& rm);

    void update(float dt, std::vector<Obstacle>& obstacles);
    void reset();
    void draw(sf::RenderWindow& window);

private:

    RessourcesManager& rm;
    float spawnTimer;
    float spawnInterval;
    float elapsedTime;

    float baseSpeed;
    float speedMultiplier;
    float maxSpeedMultiplier;

    std::mt19937 gen;
    std::uniform_real_distribution<float> gapDist;
    std::uniform_real_distribution<float> chanceDist;
    std::uniform_real_distribution<float> laserYDist;

    void spawnObstacle(std::vector<Obstacle>& obstacles);
};