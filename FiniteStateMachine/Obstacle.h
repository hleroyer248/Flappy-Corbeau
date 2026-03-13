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
    // Ajout des index topIdx et botIdx au constructeur
    Obstacle(float startX, float gapY, float gapHeight, const RessourcesManager& rm, ObstacleType type, int topIdx, int botIdx);
    void updatePositions();
    void update(float dt);
    void shift(float distance);
    CollisionBox getTopCollisionBox() const;
    CollisionBox getBottomCollisionBox() const;
    const sf::Sprite& getTopSprite() const;
    const sf::Sprite& getBottomSprite() const;
    float getX() const;
    float getWidth() const;
    bool isPassed() const;
    void setPassed(bool p);

private:
    sf::Sprite topSprite;
    sf::Sprite bottomSprite;
    float x;
    bool passed;
    float width;
    ObstacleType type;
    float baseGapY;
    float gapHeight;
    float time;
    float topTexHeight;

    const sf::Image* topImage;
    const sf::Image* bottomImage;
};