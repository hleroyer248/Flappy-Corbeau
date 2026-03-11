#include "Obstacle.h"
#include <cmath>

Obstacle::Obstacle(float startX, float gapY, float gapH, const RessourcesManager& rm, ObstacleType t)
    : topSprite(rm.getTopPipeTexture()), bottomSprite(rm.getBottomPipeTexture()),
    x(startX), passed(false), type(t), baseGapY(gapY), gapHeight(gapH), time(0.f) {
    width = static_cast<float>(rm.getTopPipeTexture().getSize().x);
    topTexHeight = static_cast<float>(rm.getTopPipeTexture().getSize().y);
    updatePositions();
}

void Obstacle::updatePositions() {
    if (type == ObstacleType::ParMouv) {
        float currentGapY = baseGapY + std::sin(time * 3.f) * 90.f;
        float topY = currentGapY - topTexHeight;
        float bottomY = currentGapY + gapHeight;
        topSprite.setPosition({ x, topY });
        bottomSprite.setPosition({ x, bottomY });
    }
    else if (type == ObstacleType::MachMouv) {
        float offset = std::sin(time * 1.5f) * 70.f;
        float topY = baseGapY - topTexHeight + offset;
        float bottomY = baseGapY + gapHeight - offset;
        topSprite.setPosition({ x, topY });
        bottomSprite.setPosition({ x, bottomY });
    }
    else {
        float topY = baseGapY - topTexHeight;
        float bottomY = baseGapY + gapHeight;
        topSprite.setPosition({ x, topY });
        bottomSprite.setPosition({ x, bottomY });
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
