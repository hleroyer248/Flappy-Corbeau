#include "Player.h"

Player::Player(const RessourcesManager& rm) : sprite(rm.getPlayerTexture()) {
    sf::Vector2u size = rm.getPlayerTexture().getSize();
    sprite.setOrigin({ size.x / 2.f, size.y / 2.f });

    // Commit Ghost - debut
    normalTexture = &rm.getPlayerTexture();
    ghostTexture = &rm.getGhostPlayerTexture();
    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;
    // Commit Ghost - fin

    reset();
}

void Player::reset() {
    sprite.setPosition({ 100.f, 300.f });
    velocity = { 0.f, 0.f };

    // Commit Ghost - debut
    sprite.setTexture(*normalTexture);
    sprite.setColor(sf::Color(255, 255, 255, 255));
    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;
    // Commit Ghost - fin
}

void Player::flap() {
    velocity.y = -350.f;
}

void Player::update(float dt) {
    velocity.y += 1200.f * dt;
    sprite.move(velocity * dt);
}

// Commit Ghost - debut
void Player::activateGhost() {
    ghostActive = true;
    ghostTimer = GHOST_DURATION;
    sprite.setTexture(*ghostTexture);
}

void Player::updateGhost(float dt) {
    if (ghostActive) {
        ghostTimer -= dt;

        if (ghostTimer <= 0.f) {
            ghostActive = false;
            ghostCooldownTimer = GHOST_COOLDOWN_MAX;
            sprite.setTexture(*normalTexture);
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }
        else if (ghostTimer <= 2.0f) {
            if (static_cast<int>(ghostTimer * 10) % 2 == 0) {
                sprite.setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
            }
            else {
                sprite.setColor(sf::Color(255, 255, 255, 255)); // Opaque
            }
        }
    }
    else if (ghostCooldownTimer > 0.f) {
        ghostCooldownTimer -= dt;
    }
}

bool Player::isGhost() const {
    return ghostActive;
}

bool Player::canActivateGhost() const {
    return !ghostActive && ghostCooldownTimer <= 0.f;
}
// Commit Ghost - fin

CollisionBox Player::getCollisionBox() const {
    return CollisionBox(sprite.getGlobalBounds());
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}

const sf::Vector2f& Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setSkin(const sf::Texture& texture)
{
    // Commit BugFix Ghost - debut
    normalTexture = &texture;
    // Commit BugFix Ghost - fin

    sprite.setTexture(texture);
}
