#include "Obstacle.h"
#include <cmath>

Obstacle::Obstacle(float startX, float gapY, float gapH, const RessourcesManager& rm, bool moving)
    : topSprite(rm.getTopPipeTexture()), bottomSprite(rm.getBottomPipeTexture()),
    x(startX), passed(false), isMoving(moving), baseGapY(gapY), gapHeight(gapH), time(0.f) {
    width = static_cast<float>(rm.getTopPipeTexture().getSize().x);
    topTexHeight = static_cast<float>(rm.getTopPipeTexture().getSize().y);
    updatePositions();
}

void Obstacle::updatePositions() {
    float currentGapY = baseGapY;
    if (isMoving) {
        currentGapY += std::sin(time * 3.f) * 90.f;
    }
    float topY = currentGapY - topTexHeight;
    topSprite.setPosition({ x, topY });
    float bottomY = currentGapY + gapHeight;
    bottomSprite.setPosition({ x, bottomY });
}

void Obstacle::update(float dt) {
    x -= 200.f * dt;
    if (isMoving) {
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