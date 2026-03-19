#include "Player.h"

Player::Player(const RessourcesManager& rm) : sprite(rm.getPlayerTexture()) {

    normalTexture = &rm.getPlayerTexture();
    ghostTexture = &rm.getGhostPlayerTexture();
    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;

    defaultPlayerTexture = &rm.getPlayerTexture();
    isAnimated = true;
    totalFrames = 7;
    currentFrame = 0;
    animationTimer = 0.f;
    frameDuration = 0.08f;

    normalImage = normalTexture->copyToImage();
    ghostImage = ghostTexture->copyToImage();
    currentImage = &normalImage;

    reset();
}

void Player::applyCurrentTexture(const sf::Texture* tex) {
    sprite.setTexture(*tex);
    sf::Vector2u size = tex->getSize();

    if (tex == ghostTexture) currentImage = &ghostImage;
    else currentImage = &normalImage;

    if (tex == defaultPlayerTexture || tex == ghostTexture) {
        isAnimated = true;
        totalFrames = 7;

        frameWidth = size.x / 3;
        frameHeight = size.y / 3;

        int col = currentFrame % 3;
        int row = currentFrame / 3;

        sprite.setTextureRect(sf::IntRect({ col * frameWidth, row * frameHeight }, { frameWidth, frameHeight }));

    }
    else {
        isAnimated = false;
        totalFrames = 1;
        frameWidth = static_cast<int>(size.x);
        frameHeight = static_cast<int>(size.y);

        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));
    }

    sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    float visualSize = 115.f;
    sprite.setScale({ visualSize / frameWidth, visualSize / frameHeight });
}

void Player::reset() {
    sprite.setPosition({ 300.f, 540.f });
    velocity = { 0.f, 0.f };

    ghostActive = false;
    ghostTimer = 0.f;
    ghostCooldownTimer = 0.f;
    sprite.setColor(skinColor);

    currentFrame = 0;
    animationTimer = 0.f;
    applyCurrentTexture(normalTexture);
}

void Player::flap() {
    velocity.y = -650.f;
}

void Player::update(float dt) {
    velocity.y += 2200.f * dt;
    sprite.move(velocity * dt);

    if (isAnimated) {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer -= frameDuration;
            currentFrame = (currentFrame + 1) % totalFrames;

            int col = currentFrame % 3;
            int row = currentFrame / 3;

            sprite.setTextureRect(sf::IntRect({ col * frameWidth, row * frameHeight }, { frameWidth, frameHeight }));
        }
    }
    if (rainbowActive) { // ou mieux: flag rainbow
        trailTimer += dt;

        if (trailTimer >= 0.05f) {
            trailTimer = 0.f;

            trail.emplace_back(sprite, 0.5f);
        }
    }
    for (auto& t : trail) {
        t.lifetime -= dt;

        float alpha = (t.lifetime / 0.5f) * 255.f;
        sf::Color c = t.sprite.getColor();
        c.a = static_cast<uint8_t>(alpha);
        t.sprite.setColor(c);
    }

    // suppression
    trail.erase(
        std::remove_if(trail.begin(), trail.end(),
            [](const TrailPoint& t) {
                return t.lifetime <= 0.f;
            }),
        trail.end()
    );
}

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
            sprite.setColor(skinColor);
        }
        else if (ghostTimer <= 2.0f) {
            if (static_cast<int>(ghostTimer * 10) % 2 == 0) {
                sprite.setColor(sf::Color(skinColor.r, skinColor.g, skinColor.b, 128));
            }
            else {
                sprite.setColor(skinColor);
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

CollisionBox Player::getCollisionBox() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float padX = bounds.size.x * 0.15f;
    float padY = bounds.size.y * 0.15f;

    bounds.size.x -= (padX * 2.f);
    bounds.size.y -= (padY * 2.f);

    bounds.position.x += padX;
    bounds.position.y += padY;

    CollisionBox cb(sprite, *currentImage);
    cb.setRect(bounds);

    return cb;
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setSkin(const sf::Texture& texture)
{
    normalTexture = &texture;

    normalImage = texture.copyToImage();

    if (!ghostActive) {
        applyCurrentTexture(normalTexture);
        sprite.setColor(skinColor);
    }
}


float Player::getCooldownRatio() const {
    if (ghostCooldownTimer <= 0.f) return 0.f;
    return ghostCooldownTimer / GHOST_COOLDOWN_MAX;
}

void Player::setSkinColor(const sf::Color& color)
{
    skinColor = color;
    sprite.setColor(skinColor);
}

const std::vector<Player::TrailPoint>& Player::getTrail() const {
    return trail;
}