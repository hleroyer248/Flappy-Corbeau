#include "Obstacle.h"
#include <cmath>
#include <random>
#include <iostream>

Obstacle::Obstacle(float startX, float gapY, float gapH, const RessourcesManager& rm, ObstacleType t)
    : topSprite(rm.getTopPipeTexture()), bottomSprite(rm.getBottomPipeTexture()),
    x(startX), passed(false), type(t), baseGapY(gapY), gapHeight(gapH), time(0.f) 
{
    width = static_cast<float>(rm.getTopPipeTexture().getSize().x);
    topTexHeight = static_cast<float>(rm.getTopPipeTexture().getSize().y);
    // Choix aléatoire de la statue
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 3);

    int statueIndex = dist(gen);

    if (statueIndex == 1)
        bottomSprite.setTexture(rm.getStatue1Texture());
    else if (statueIndex == 2)
        bottomSprite.setTexture(rm.getStatue2Texture());
    else
        bottomSprite.setTexture(rm.getStatue3Texture());
    bottomSprite.setScale({ 0.1f, 0.1f });

    updatePositions();
}

void Obstacle::updatePositions() {
    if (type == ObstacleType::ParMouv) {
        float currentGapY = baseGapY + std::sin(time * 3.f) * 90.f;
        float topY = currentGapY - topTexHeight;
        float groundY = 600.f - bottomSprite.getGlobalBounds().size.y;
        bottomSprite.setPosition({ x, groundY });
        topSprite.setPosition({ x, topY });
    }
    else if (type == ObstacleType::MachMouv) {
        float offset = std::sin(time * 1.5f) * 70.f;
        float topY = baseGapY - topTexHeight + offset;
        float groundY = 600.f - bottomSprite.getGlobalBounds().size.y;
        bottomSprite.setPosition({ x, groundY });
        topSprite.setPosition({ x, topY });
    }
    else {
        float topY = baseGapY - topTexHeight;
        float bottomY = baseGapY + gapHeight;
        float groundY = 600.f - bottomSprite.getGlobalBounds().size.y;
        bottomSprite.setPosition({ x, groundY });
    }
}

void Obstacle::update(float dt) {
    x -= 200.f * dt;
    if (type != ObstacleType::Normal) {
        time += dt;
    }
    updatePositions();
}

void Obstacle::shift(float distance) {
    x -= distance;
    updatePositions();
}

CollisionBox Obstacle::getTopCollisionBox() const {
    return CollisionBox(topSprite.getGlobalBounds());
}

CollisionBox Obstacle::getBottomCollisionBox() const {
    return CollisionBox(bottomSprite.getGlobalBounds());
}

const sf::Sprite& Obstacle::getTopSprite() const { return topSprite; }
const sf::Sprite& Obstacle::getBottomSprite() const { return bottomSprite; }
float Obstacle::getX() const { return x; }
float Obstacle::getWidth() const { return width; }
bool Obstacle::isPassed() const { return passed; }
void Obstacle::setPassed(bool p) { passed = p; }
