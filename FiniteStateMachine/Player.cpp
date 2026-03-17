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

    // Commit Pixel-Perfect - debut
    normalImage = normalTexture->copyToImage();
    ghostImage = ghostTexture->copyToImage();
    currentImage = &normalImage;
    // Commit Pixel-Perfect - fin

    reset();
}

void Player::applyCurrentTexture(const sf::Texture* tex) {
    sprite.setTexture(*tex);
    sf::Vector2u size = tex->getSize();

    // Commit Pixel-Perfect - debut
    if (tex == ghostTexture) currentImage = &ghostImage;
    else currentImage = &normalImage;
    // Commit Pixel-Perfect - fin

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
    float visualSize = 115.f;
    sprite.setScale({ visualSize / frameWidth, visualSize / frameHeight });
    // Commit Visual Size - fin
}

void Player::reset() {
    sprite.setPosition({ 300.f, 540.f });
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
    velocity.y = -650.f;
}

void Player::update(float dt) {
    velocity.y += 2200.f * dt;
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
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Ajout d'un padding pour le joueur (15% de réduction)
    float padX = bounds.size.x * 0.15f;
    float padY = bounds.size.y * 0.15f;

    bounds.size.x -= (padX * 2.f);
    bounds.size.y -= (padY * 2.f);

    // Centrage de la hitbox sur le corps du joueur
    bounds.position.x += padX;
    bounds.position.y += padY;

    // Création de la CollisionBox avec le rectangle réduit
    CollisionBox cb(sprite, *currentImage);
    cb.setRect(bounds);

    return cb;
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}

// Correction SFML 3.0 : Retourne une valeur et non une référence
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setSkin(const sf::Texture& texture)
{
    // Commit BugFix Ghost - debut
    normalTexture = &texture;
    // Commit BugFix Ghost - fin

    // Commit Pixel-Perfect - debut
    normalImage = texture.copyToImage();
    // Commit Pixel-Perfect - fin

    // Commit Crow - debut
    if (!ghostActive) {
        applyCurrentTexture(normalTexture);
    }
    // Commit Crow - fin
}


float Player::getCooldownRatio() const {
    if (ghostCooldownTimer <= 0.f) return 0.f;
    return ghostCooldownTimer / GHOST_COOLDOWN_MAX;
}