#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>

#include "RessourcesManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Obstacle.h"
#include "MainMenu.h"
#include "Boutique.h"
#include "OptionMenu.h"

class GameEvent {

public:

    enum class EventState{Normal,Warning,Laser };

    EventState state;

    bool shouldClearObstacles = false;
    bool laserDodgedThisFrame = false;

    int obstaclesPassed;
    int lasersDodged;

    float warningTimer;
    float laserTimer;
    void addObstaclePassed(std::vector<Obstacle>& obstacles);

    sf::RectangleShape warningRect;
    sf::Sprite laserSprite;

GameEvent(RessourcesManager& rm);


void update(float dt, std::vector<Obstacle>& obstacles);
void reset();
void draw(sf::RenderWindow& window);

//void update(float dt, std::vector<Obstacle>& obstacles, float playerX, int score);

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

    void spawnObstacle(std::vector<Obstacle>& obstacles);
};
