#include "Obstacle.h"
#include <cmath>
#include <random> // Nécessaire pour std::random_device et std::mt19937

Obstacle::Obstacle(float startX, float gapY, float gapH, const RessourcesManager& rm, ObstacleType t, int topIdx, int botIdx)
    : topSprite(rm.getTopPipeTexture(topIdx)),
    bottomSprite(rm.getBottomPipeTexture(botIdx)),
    x(startX), passed(false), type(t), baseGapY(gapY), gapHeight(gapH), time(0.f) {

    topImage = &rm.getTopPipeImage(topIdx);
    bottomImage = &rm.getBottomPipeImage(botIdx);

    float originalTopTexHeight = static_cast<float>(topSprite.getTexture().getSize().y);
    float originalTopTexWidth = static_cast<float>(topSprite.getTexture().getSize().x);

    // MODIFICATION ICI : On élargit les tuyaux pour le 1080p
    float desiredWidth = 650.f;  // Passé de 150.f à 250.f
    float desiredHeight = 1200.f;

    float scaleX = desiredWidth / originalTopTexWidth;
    float scaleY = desiredHeight / originalTopTexHeight;

    topSprite.setScale({ scaleX, scaleY });
    bottomSprite.setScale({ scaleX, scaleY });

    topTexHeight = desiredHeight;
    width = desiredWidth;

    updatePositions();
}

void Obstacle::updatePositions() {
    if (type == ObstacleType::ParMouv) {
        float currentGapY = baseGapY + std::sin(time * 3.f) * 120.f;
        float topY = currentGapY - topTexHeight;
        float bottomY = currentGapY + gapHeight;
        topSprite.setPosition({ x, topY });
        bottomSprite.setPosition({ x, bottomY });
    }
    else if (type == ObstacleType::MachMouv) {
        float offset = std::sin(time * 1.5f) * 80.f;
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
    x -= 400.f * dt;
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
    sf::FloatRect bounds = topSprite.getGlobalBounds();

    // Padding : on réduit la hitbox de 15% sur les côtés et 5% sur le bord piquant
    float paddingX = bounds.size.x * 0.35f;
    float paddingY = bounds.size.y * 0.09f;

    bounds.size.x -= (paddingX * 2.f);
    bounds.size.y -= paddingY; // Réduit seulement le bas (le bord du gap)

    bounds.position.x += paddingX - 25;

    CollisionBox cb(topSprite, *topImage);
    cb.setRect(bounds);
    return cb;
}

CollisionBox Obstacle::getBottomCollisionBox() const {
    sf::FloatRect bounds = bottomSprite.getGlobalBounds();

    float paddingX = bounds.size.x * 0.35f;
    float paddingY = bounds.size.y * 0.02f;

    bounds.size.x -= (paddingX * 2.f);
    bounds.size.y -= paddingY;

    bounds.position.x += paddingX - 25;
    bounds.position.y += paddingY; // Décale le début de la collision vers le bas

    CollisionBox cb(bottomSprite, *bottomImage);
    cb.setRect(bounds);
    return cb;
}

const sf::Sprite& Obstacle::getTopSprite() const { return topSprite; }
const sf::Sprite& Obstacle::getBottomSprite() const { return bottomSprite; }
float Obstacle::getX() const { return x; }
float Obstacle::getWidth() const { return width; }
bool Obstacle::isPassed() const { return passed; }
void Obstacle::setPassed(bool p) { passed = p; }