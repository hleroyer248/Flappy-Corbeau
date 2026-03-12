#include "Player.h"

Player::Player(const RessourcesManager& rm) : sprite(rm.getPlayerTexture()) {

    // Commit Ghost - debut
    normalTexture = &rm.getPlayerTexture();
    ghostTexture = &rm.getGhostPlayerTexture();
    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;
    // Commit Ghost - fin

    // Commit Crow - debut
    defaultPlayerTexture = &rm.getPlayerTexture();
    isAnimated = true;
    totalFrames = 7;
    currentFrame = 0;
    animationTimer = 0.f;
    frameDuration = 0.08f;
    // Commit Crow - fin

    reset();
}

void Player::applyCurrentTexture(const sf::Texture* tex) {
    sprite.setTexture(*tex);
    sf::Vector2u size = tex->getSize();

    if (tex == defaultPlayerTexture) {
        isAnimated = true;
        totalFrames = 7;

        // Commit Grid Anim - debut
        frameWidth = size.x / 3;
        frameHeight = size.y / 3;

        int col = currentFrame % 3;
        int row = currentFrame / 3;

        sprite.setTextureRect(sf::IntRect({ col * frameWidth, row * frameHeight }, { frameWidth, frameHeight }));
        // Commit Grid Anim - fin

    }
    else {
        isAnimated = false;
        totalFrames = 1;
        frameWidth = static_cast<int>(size.x);
        frameHeight = static_cast<int>(size.y);

        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));
    }

    sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    // Commit Visual Size - debut
    // On force la taille du sprite à une taille plus grande (ex: 64x64 pixels)
    // Tu peux changer cette valeur (ex: 48.f, 80.f) pour affiner le rendu
    float visualSize = 64.f;
    sprite.setScale({ visualSize / frameWidth, visualSize / frameHeight });
    // Commit Visual Size - fin
}

void Player::reset() {
    sprite.setPosition({ 100.f, 300.f });
    velocity = { 0.f, 0.f };

    // Commit Ghost - debut
    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;
    sprite.setColor(sf::Color(255, 255, 255, 255));
    // Commit Ghost - fin

    // Commit Crow - debut
    currentFrame = 0;
    animationTimer = 0.f;
    applyCurrentTexture(normalTexture);
    // Commit Crow - fin
}

void Player::flap() {
    velocity.y = -350.f;
}

void Player::update(float dt) {
    velocity.y += 1200.f * dt;
    sprite.move(velocity * dt);

    // Commit Crow - debut
    if (isAnimated) {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer -= frameDuration;
            currentFrame = (currentFrame + 1) % totalFrames;

            // Commit Grid Anim - debut
            int col = currentFrame % 3;
            int row = currentFrame / 3;

            sprite.setTextureRect(sf::IntRect({ col * frameWidth, row * frameHeight }, { frameWidth, frameHeight }));
            // Commit Grid Anim - fin
        }
    }
    // Commit Crow - fin
}

// Commit Ghost - debut
void Player::activateGhost() {
    ghostActive = true;
    ghostTimer = GHOST_DURATION;
    applyCurrentTexture(ghostTexture);
}

void Player::updateGhost(float dt) {
    if (ghostActive) {
        ghostTimer -= dt;

        if (ghostTimer <= 0.f) {
            ghostActive = false;
            ghostCooldownTimer = GHOST_COOLDOWN_MAX;
            applyCurrentTexture(normalTexture);
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }
        else if (ghostTimer <= 2.0f) {
            if (static_cast<int>(ghostTimer * 10) % 2 == 0) {
                sprite.setColor(sf::Color(255, 255, 255, 128));
            }
            else {
                sprite.setColor(sf::Color(255, 255, 255, 255));
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
    // Commit Visual Size - debut
    sf::Vector2f pos = sprite.getPosition();
    return CollisionBox(sf::FloatRect({ pos.x - 16.f, pos.y - 16.f }, { 32.f, 32.f }));
    // Commit Visual Size - fin
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

    // Commit Crow - debut
    if (!ghostActive) {
        applyCurrentTexture(normalTexture);
    }
    // Commit Crow - fin
}