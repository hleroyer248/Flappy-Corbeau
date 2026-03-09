#include "Player.h"

Player::Player(const RessourcesManager& rm) : sprite(rm.getPlayerTexture()) {
    sf::Vector2u size = rm.getPlayerTexture().getSize();
    sprite.setOrigin({ size.x / 2.f, size.y / 2.f });
    reset();
}

void Player::reset() {
    sprite.setPosition({ 100.f, 300.f });
    velocity = { 0.f, 0.f };
    dashCooldownTimer = 0.f;
}

void Player::flap() {
    velocity.y = -350.f;
}

void Player::update(float dt) {
    velocity.y += 1200.f * dt;
    sprite.move(velocity * dt);
}

void Player::dash() {
    dashCooldownTimer = DASH_COOLDOWN_MAX;
}

void Player::updateDashCooldown(float dt) {
    if (dashCooldownTimer > 0.f) {
        dashCooldownTimer -= dt;
    }
}

bool Player::canDash() const {
    return dashCooldownTimer <= 0.f;
}

float Player::getDashDistance() const {
    return DASH_DISTANCE;
}

CollisionBox Player::getCollisionBox() const {
    return CollisionBox(sprite.getGlobalBounds());
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}

const sf::Vector2f& Player::getPosition() const {
    return sprite.getPosition();
}