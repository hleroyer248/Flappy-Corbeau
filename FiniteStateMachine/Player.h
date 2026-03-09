#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"
#include "CollisionBox.h"

class Player {
public:
    Player(const RessourcesManager& rm);
    void reset();
    void flap();
    void update(float dt);
    void dash();
    void updateDashCooldown(float dt);
    bool canDash() const;
    float getDashDistance() const;
    CollisionBox getCollisionBox() const;
    const sf::Sprite& getSprite() const;
    const sf::Vector2f& getPosition() const;

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float dashCooldownTimer;
    const float DASH_COOLDOWN_MAX = 2.5f;
    const float DASH_DISTANCE = 150.f;
};