#include "GameEvent.h"
#include <iostream>

GameEvent::GameEvent(RessourcesManager& rm, AudioManager& am) :
    rm(rm),
    am(am),
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
    waveTimer(0.f),
    currentWaveDuration(0.f),
    screenShakeIntensity(0.f),
    laserAnimTimer(0.f),
    currentLaserFrame(0) {

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
    waveTimer = 0.f;
    screenShakeIntensity = 0.f;
    activeLasers.clear();
    shouldClearObstacles = false;
}

void GameEvent::update(float dt, std::vector<Obstacle>& obstacles) {
    if (activeLasers.empty()) {
        setupWave(0);
    }
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
    else {
        waveTimer += dt;

        laserAnimTimer += dt;
        float frameDuration = 0.12f;
        if (laserAnimTimer >= frameDuration) {
            laserAnimTimer -= frameDuration;
            currentLaserFrame = (currentLaserFrame + 1) % rm.getLaserFrameCount();
        }

        sf::IntRect currentRect = rm.getLaserRect(currentLaserFrame);
        bool anyFiring = false;
        bool anyWarning = false;
        for (auto& l : activeLasers) {
            float warningStart = l.startWarningAt;

            // 🔥 Joue le son EXACTEMENT quand CE laser commence
            if (!l.soundPlayed && waveTimer >= warningStart) {
                am.playLaserSound();
                l.soundPlayed = true;
            }
            if (waveTimer >= l.startWarningAt && waveTimer < l.startWarningAt + l.warningDuration) {
                l.isWarning = true;
                l.isFiring = false;
                anyWarning = true;
            }
            else if (waveTimer >= l.startWarningAt + l.warningDuration && waveTimer < l.startWarningAt + l.warningDuration + l.activeDuration) {
                l.isWarning = false;
                l.isFiring = true;
                anyFiring = true;
            }
            else {
                l.isWarning = false;
                l.isFiring = false;
            }

            if (l.sweepSpeed != 0.f && l.isFiring) {
                float newY = l.sprite.getPosition().y + l.sweepSpeed * dt;

                if (newY < l.sweepMinY) { newY = l.sweepMinY; l.sweepSpeed *= -1.f; }
                if (newY > l.sweepMaxY) { newY = l.sweepMaxY; l.sweepSpeed *= -1.f; }

                l.sprite.setPosition({ l.sprite.getPosition().x, newY });
            }

            if (currentRect.size.x > 0) {
                float desiredThickness = 60.f;
                float scaleY = desiredThickness / static_cast<float>(currentRect.size.y);
                float scaleX = (l.isDiagonal ? 2203.f : 1920.f) / static_cast<float>(currentRect.size.x);
                l.sprite.setTextureRect(currentRect);
                l.sprite.setScale({ scaleX, scaleY });
            }
        }

        screenShakeIntensity = 0.f;
        if (lasersDodged == 4 && waveTimer >= 3.5f && waveTimer < 5.0f) {
            screenShakeIntensity = 15.f;
        }

        if (anyFiring) state = EventState::Laser;
        else state = EventState::Warning;

        if (waveTimer >= currentWaveDuration) {
            lasersDodged++;
            laserDodgedThisFrame = true;

            if (lasersDodged >= 5) {
                lasersDodged = 0;
                state = EventState::Normal;
                activeLasers.clear();
            }
            else {
                setupWave(lasersDodged);
            }
        }
    }
}

void GameEvent::setupWave(int waveIndex) {
    activeLasers.clear();
    waveTimer = 0.f;
    screenShakeIntensity = 0.f;

    if (waveIndex == 0) {
        currentWaveDuration = 3.5f;
        addHorizontalLaser(1.0f, 2.5f, 0.f);
        addHorizontalLaser(1.0f, 2.5f, 0.f);
        ensureSpacing(250.f, 0.f);
    }
    else if (waveIndex == 1) {
        currentWaveDuration = 3.5f;
        addHorizontalLaser(1.0f, 2.5f, 0.f);
        addHorizontalLaser(1.0f, 2.5f, 0.f);
        addHorizontalLaser(1.0f, 2.5f, 0.f);
        ensureSpacing(200.f, 0.f);
    }
    else if (waveIndex == 2) {
        currentWaveDuration = 4.5f;
        addHorizontalLaser(1.0f, 3.5f, 0.f, 200.f, 100.f, 750.f);
    }
    else if (waveIndex == 3) {
        currentWaveDuration = 7.5f;

        addHorizontalLaser(1.0f, 2.0f, 0.f);
        addHorizontalLaser(1.0f, 2.0f, 0.f);
        ensureSpacing(250.f, 0.f);

        addHorizontalLaser(1.0f, 2.0f, 2.0f);
        addHorizontalLaser(1.0f, 2.0f, 2.0f);
        ensureSpacing(250.f, 2.0f);

        addHorizontalLaser(1.0f, 2.0f, 4.0f);
        addHorizontalLaser(1.0f, 2.0f, 4.0f);
        ensureSpacing(250.f, 4.0f);
    }
    else if (waveIndex == 4) {
        currentWaveDuration = 5.5f;

        addHorizontalLaser(1.0f, 2.0f, 0.f, 150.f, 100.f, 400.f);
        addHorizontalLaser(1.0f, 2.0f, 0.f, -150.f, 600.f, 900.f);

        addHorizontalLaser(0.5f, 1.5f, 3.0f);
        addHorizontalLaser(0.5f, 1.5f, 3.0f);
        addHorizontalLaser(0.5f, 1.5f, 3.0f);
        ensureSpacing(200.f, 3.0f);

        addDiagonalLaser(0.5f, 1.5f, 3.0f, 1440.f);
        addDiagonalLaser(0.5f, 1.5f, 3.0f, 400.f);
    }

    state = EventState::Warning;
}

void GameEvent::addObstaclePassed(std::vector<Obstacle>& obstacles) {
    obstaclesPassed++;

    if (obstaclesPassed >= 10 && state == EventState::Normal) {
        obstaclesPassed = 0;
        shouldClearObstacles = true;
        setupWave(0);
    }
}

void GameEvent::draw(sf::RenderWindow& window) {
    sf::View originalView = window.getView();

    if (screenShakeIntensity > 0.f) {
        sf::View shakeView = originalView;
        std::uniform_real_distribution<float> shakeDist(-screenShakeIntensity, screenShakeIntensity);
        shakeView.move({ shakeDist(gen), shakeDist(gen) });
        window.setView(shakeView);
    }

    for (const auto& l : activeLasers) {
        if (l.isWarning) window.draw(l.warningRect);
        if (l.isFiring) window.draw(l.sprite);
    }

    window.setView(originalView);
}

std::vector<CollisionBox> GameEvent::getLaserCollisionBoxes() const {
    std::vector<CollisionBox> boxes;

    for (const auto& l : activeLasers) {
        if (l.isFiring) {
            if (!l.isDiagonal) {
                boxes.push_back(CollisionBox(l.sprite.getGlobalBounds()));
            }
            else {
                int numBoxes = 50;
                float dx = -1920.f / numBoxes;
                float dy = 1080.f / numBoxes;

                float startX = l.sprite.getPosition().x;

                for (int i = 0; i <= numBoxes; ++i) {
                    float cx = startX + dx * i;
                    float cy = 0.f + dy * i;
                    boxes.push_back(CollisionBox(sf::FloatRect({ cx - 22.5f, cy - 22.5f }, { 45.f, 45.f })));
                }
            }
        }
    }
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

void GameEvent::addHorizontalLaser(float warnDur, float actDur, float startWarn, float sweepSpd, float sweepMin, float sweepMax) {
    LaserEntity l(rm.getLaserTexture());
    l.isDiagonal = false;
    l.warningDuration = warnDur;
    l.activeDuration = actDur;
    l.startWarningAt = startWarn;
    l.sweepSpeed = sweepSpd;
    l.sweepMinY = sweepMin;
    l.sweepMaxY = sweepMax;

    l.warningRect.setSize({ 1920.f, 60.f });
    l.warningRect.setFillColor(sf::Color(255, 0, 0, 150));
    l.warningRect.setOrigin({ 0.f, 0.f });

    sf::IntRect firstRect = rm.getLaserRect(0);
    if (firstRect.size.x > 0) {
        float scaleX = 1920.f / static_cast<float>(firstRect.size.x);
        float scaleY = 60.f / static_cast<float>(firstRect.size.y);
        l.sprite.setOrigin({ 0.f, 0.f });
        l.sprite.setTextureRect(firstRect);
        l.sprite.setScale({ scaleX, scaleY });
    }

    float startY = laserYDist(gen);

    if (sweepSpd != 0.f) {
        startY = std::max(sweepMin, std::min(startY, sweepMax));
    }

    l.sprite.setPosition({ 0.f, startY });
    l.warningRect.setPosition({ 0.f, startY });

    activeLasers.push_back(l);
}

void GameEvent::addDiagonalLaser(float warnDur, float actDur, float startWarn, float startX) {
    LaserEntity l(rm.getLaserTexture());
    l.isDiagonal = true;
    l.warningDuration = warnDur;
    l.activeDuration = actDur;
    l.startWarningAt = startWarn;

    l.warningRect.setSize({ 2203.f, 60.f });
    l.warningRect.setFillColor(sf::Color(255, 0, 0, 150));
    l.warningRect.setOrigin({ 0.f, 30.f });
    l.warningRect.setPosition({ startX, 0.f });
    l.warningRect.setRotation(sf::degrees(150.64f));

    sf::IntRect firstRect = rm.getLaserRect(0);
    if (firstRect.size.x > 0) {
        float scaleX = 2203.f / static_cast<float>(firstRect.size.x);
        float scaleY = 60.f / static_cast<float>(firstRect.size.y);
        l.sprite.setOrigin({ 0.f, static_cast<float>(firstRect.size.y) / 2.f });
        l.sprite.setPosition({ startX, 0.f });
        l.sprite.setRotation(sf::degrees(150.64f));
        l.sprite.setTextureRect(firstRect);
        l.sprite.setScale({ scaleX, scaleY });
    }

    activeLasers.push_back(l);
}

void GameEvent::ensureSpacing(float minDist, float targetStartTime) {
    bool ok = false;
    int attempts = 0;
    while (!ok && attempts < 100) {
        ok = true;
        for (size_t i = 0; i < activeLasers.size(); ++i) {
            if (activeLasers[i].startWarningAt != targetStartTime || activeLasers[i].isDiagonal) continue;

            for (size_t j = i + 1; j < activeLasers.size(); ++j) {
                if (activeLasers[j].startWarningAt != targetStartTime || activeLasers[j].isDiagonal) continue;

                if (std::abs(activeLasers[i].sprite.getPosition().y - activeLasers[j].sprite.getPosition().y) < minDist) {
                    ok = false;
                    float newY = laserYDist(gen);
                    activeLasers[j].sprite.setPosition({ 0.f, newY });
                    activeLasers[j].warningRect.setPosition({ 0.f, newY });
                }
            }
        }
        attempts++;
    }
}
