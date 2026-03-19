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

struct LaserEntity {
    sf::RectangleShape warningRect;
    sf::Sprite sprite;

    bool isDiagonal = false;

    float warningDuration = 1.0f;
    float activeDuration = 2.5f;
    float startWarningAt = 0.f;
    float sweepSpeed = 0.f;

    float sweepMinY = 100.f;
    float sweepMaxY = 800.f;

    bool isWarning = false;
    bool isFiring = false;

    bool soundPlayed = false;

    LaserEntity(const sf::Texture& tex) : sprite(tex) {}
};


class GameEvent {

public:
    AudioManager& am;

    enum class EventState { Normal, Warning, Laser };

    EventState state;

    bool shouldClearObstacles = false;
    bool laserDodgedThisFrame = false;

    std::vector<CollisionBox> getLaserCollisionBoxes() const;

    int obstaclesPassed;
    int lasersDodged;

    float waveTimer;
    float currentWaveDuration;
    float screenShakeIntensity;

    float laserAnimTimer;
    int currentLaserFrame;

    float laserProbability;

    void addObstaclePassed(std::vector<Obstacle>& obstacles);

    std::vector<LaserEntity> activeLasers;

    bool isLaserActive() const;

    GameEvent(RessourcesManager& rm, AudioManager& am);

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

    void setupWave(int waveIndex);

    void addHorizontalLaser(float warnDur, float actDur, float startWarn, float sweepSpd = 0.f, float sweepMin = 100.f, float sweepMax = 800.f);
    void addDiagonalLaser(float warnDur, float actDur, float startWarn, float startX);
    void ensureSpacing(float minDist, float targetStartTime);
};
