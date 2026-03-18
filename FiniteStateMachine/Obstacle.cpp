#include "Obstacle.h"
#include <cmath>
#include <random> // Nécessaire pour std::random_device et std::mt19937

Obstacle::Obstacle(float startX, float gapY, float gapH, const RessourcesManager& rm, ObstacleType t, int topIdx, int botIdx)
    : topSprite(rm.getTopPipeTexture(topIdx)),
    bottomSprite(rm.getBottomPipeTexture(botIdx)),
    topHead(rm.getBottomPipeTexture(botIdx)),
    topBody(rm.getBottomPipeTexture(botIdx)),
    bottomHead(rm.getBottomHeadTexture(botIdx)),
    bottomBody(rm.getBottomBodyTexture(botIdx)),
    x(startX), passed(false), type(t), baseGapY(gapY), gapHeight(gapH), time(0.f) {

    topImage = &rm.getTopPipeImage(topIdx);
    bottomImage = &rm.getBottomPipeImage(botIdx);

    float headScaleAdjust[3] = { 0.97f, 0.95f, 0.93f };
    float offsetXAdjust[3] = { 4.f, 1.f, 4.f };
    float offsetYAdjust[3] = { 0.f, -4.f, -3.f };

    headAdjust = headScaleAdjust[botIdx];
    offsetX = offsetXAdjust[botIdx];
    offsetY = offsetYAdjust[botIdx];

    float globalScale = 0.20f; // Scale global des obstacle bas 

    float originalTopTexHeight = static_cast<float>(topSprite.getTexture().getSize().y);
    float originalTopTexWidth = static_cast<float>(topSprite.getTexture().getSize().x);

    // MODIFICATION ICI : On élargit les tuyaux pour le 1080p
    float desiredWidth = 650.f;  // Passé de 150.f à 250.f
    float desiredHeight = 1200.f;

    float scaleXTop = desiredWidth / originalTopTexWidth;
    float scaleYTop = desiredHeight / originalTopTexHeight;

    topSprite.setScale({ scaleXTop, scaleYTop });

    // HEAD
    float headWidth = static_cast<float>(bottomHead.getTexture().getSize().x);
    float bodyWidth = static_cast<float>(bottomBody.getTexture().getSize().x);

    float scaleXHead = (desiredWidth / headWidth) * globalScale;
    float scaleXBody = (desiredWidth / bodyWidth) * globalScale;

    float scaleXBottom = desiredWidth / headWidth;

    bottomHead.setScale({
    scaleXHead * headAdjust,
    scaleXHead * headAdjust
        });

    // BODY (étiré seulement en Y)
    float bodyHeight = bottomBody.getTexture().getSize().y;


    bottomBody.setScale({
        scaleXBody,
      scaleXBody
        });

    topTexHeight = desiredHeight;
    width = desiredWidth;

    updatePositions();
}

void Obstacle::updatePositions() {
    if (type == ObstacleType::ParMouv) {
        float currentGapY = baseGapY + std::sin(time * 3.f) * 120.f;
        float topY = currentGapY - topTexHeight;
        topSprite.setPosition({ x, topY });
        float bottomY = currentGapY + gapHeight;

        // HEAD

        bottomHead.setPosition({ x + offsetX, bottomY + offsetY });

        // BODY
        float headHeight = bottomHead.getGlobalBounds().size.y;

        float fixOffset = -12.f; 

        bottomBody.setPosition({ x, bottomY + headHeight + fixOffset });

        // scale dynamique
        float screenBottom = 1080.f;
        float bodyHeight = screenBottom - (bottomY + headHeight);

        float originalBodyHeight = bottomBody.getTexture().getSize().y;

       
    }
    else if (type == ObstacleType::MachMouv) {
        float offset = std::sin(time * 1.5f) * 80.f;

        float topY = baseGapY - topTexHeight + offset;
        float bottomY = baseGapY + gapHeight - offset;

        topSprite.setPosition({ x, topY });


        bottomHead.setPosition({ x + offsetX, bottomY + offsetY });

        float headHeight = bottomHead.getGlobalBounds().size.y;

        float fixOffset = -12.f; 

        bottomBody.setPosition({ x, bottomY + headHeight + fixOffset });

        float bodyHeight = 1080.f - (bottomY + headHeight);

        float originalBodyHeight = static_cast<float>(bottomBody.getTexture().getSize().y);

       
    }
    else {
        float topY = baseGapY - topTexHeight;
        topSprite.setPosition({ x, topY });


        float bottomY = baseGapY + gapHeight;

        // HEAD

        bottomHead.setPosition({ x + offsetX, bottomY + offsetY });

        // BODY (sous la tête)
        float headHeight = bottomHead.getTexture().getSize().y * bottomHead.getScale().y;
        float fixOffset = -12.f; // ajuste à la main

        bottomBody.setPosition({ x, bottomY + headHeight + fixOffset });

        // Calcul de la hauteur du body
        float screenBottom = 1080.f;
        float bodyHeight = screenBottom - (bottomY + headHeight);

        float originalBodyHeight = bottomBody.getTexture().getSize().y;

       
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

    float paddingX = bounds.size.x * 0.35f;
    float paddingY = bounds.size.y * 0.09f;

    bounds.size.x -= (paddingX * 2.f);
    bounds.size.y -= paddingY;

    bounds.position.x += paddingX - 25;

    CollisionBox cb(topSprite, *topImage);
    cb.setRect(bounds);
    return cb;
}

CollisionBox Obstacle::getBottomCollisionBox() const {
    sf::FloatRect bounds = bottomHead.getGlobalBounds();

    bounds.size.y = 1080.f - bounds.position.y;

    float paddingX = bounds.size.x * 0.001f;
    float paddingY = bounds.size.y * 0.02f;

    bounds.size.x -= (paddingX * 2.f);
    bounds.size.y -= paddingY;

    bounds.position.x += paddingX;
    bounds.position.y += paddingY;

    CollisionBox cb(bounds);
    return cb;
}

const sf::Sprite& Obstacle::getTopSprite() const { return topSprite; }
const sf::Sprite& Obstacle::getBottomSprite() const { return bottomSprite; }
const sf::Sprite& Obstacle::getBottomHead() const { return bottomHead; }
const sf::Sprite& Obstacle::getBottomBody() const { return bottomBody; }
float Obstacle::getX() const { return x; }
float Obstacle::getWidth() const { return width; }
bool Obstacle::isPassed() const { return passed; }
void Obstacle::setPassed(bool p) { passed = p; }