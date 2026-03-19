#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"
#include "CollisionBox.h"

class Player {
    struct TrailPoint {
        sf::Sprite sprite;
        float lifetime;

        TrailPoint(const sf::Sprite& s, float life)
            : sprite(s), lifetime(life) {
        }
    };

public:
    Player(const RessourcesManager& rm);
    void reset();
    void flap();
    void update(float dt);

    void activateGhost();
    void updateGhost(float dt);
    bool isGhost() const;
    bool canActivateGhost() const;

    CollisionBox getCollisionBox() const;
    const sf::Sprite& getSprite() const;

    sf::Vector2f getPosition() const;

    void setSkin(const sf::Texture& texture);

    float getCooldownRatio() const;

    void setSkinColor(const sf::Color& color);
    const std::vector<TrailPoint>& getTrail() const;

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;

    const sf::Texture* normalTexture;
    const sf::Texture* ghostTexture;

    bool ghostActive;
    float ghostTimer;
    float ghostCooldownTimer;
    const float GHOST_DURATION = 5.0f;
    const float GHOST_COOLDOWN_MAX = 6.0f;

    const sf::Texture* defaultPlayerTexture; 
    bool isAnimated;
    int currentFrame;
    float animationTimer;
    float frameDuration;
    int frameWidth;
    int frameHeight;
    int totalFrames;

    void applyCurrentTexture(const sf::Texture* tex);
  
    sf::Image normalImage;
    sf::Image ghostImage;
    const sf::Image* currentImage;
    sf::Color skinColor = sf::Color::White;
    float trailTimer = 0.f;
    std::vector<TrailPoint> trail;
    bool rainbowActive = false;

};
