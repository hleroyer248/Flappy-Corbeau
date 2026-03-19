#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"
#include "CollisionBox.h"

enum class ObstacleType {
    Normal,
    ParMouv,
    MachMouv
};

class Obstacle {
public:
    Obstacle(float startX, float gapY, float gapHeight, const RessourcesManager& rm, ObstacleType type, int topIdx, int botIdx);
    void updatePositions();
    void update(float dt);
    void shift(float distance);
    CollisionBox getTopCollisionBox() const;
    CollisionBox getBottomCollisionBox() const;
    const sf::Sprite& getTopSprite() const;
    const sf::Sprite& getBottomSprite() const;
    const sf::Sprite& getBottomHead() const;
    const sf::Sprite& getBottomBody() const;
    float getX() const;
    float getWidth() const;
    bool isPassed() const;
    void setPassed(bool p);

private:
    sf::Sprite topSprite;
    sf::Sprite bottomSprite;

    sf::Sprite topHead;
    sf::Sprite topBody;

    sf::Sprite bottomHead;
    sf::Sprite bottomBody;


    float x;
    bool passed;
    float width;
    ObstacleType type;
    float baseGapY;
    float gapHeight;
    float time;
    float topTexHeight;

    float offsetX;
    float offsetY;
    float headAdjust;

    const sf::Image* topImage;
    const sf::Image* bottomImage;
};
