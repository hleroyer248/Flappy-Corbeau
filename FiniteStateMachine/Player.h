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

    // Commit Ghost - debut 
    void activateGhost();
    void updateGhost(float dt);
    bool isGhost() const;
    bool canActivateGhost() const;
    // Commit Ghost - fin

    CollisionBox getCollisionBox() const;
    const sf::Sprite& getSprite() const;

    // Correction SFML 3.0 : Retourne une valeur et non une référence
    sf::Vector2f getPosition() const;

    void setSkin(const sf::Texture& texture);

    float getCooldownRatio() const;

    void setSkinColor(const sf::Color& color);

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;

    // Commit Ghost - debut
    const sf::Texture* normalTexture;
    const sf::Texture* ghostTexture;

    bool ghostActive;
    float ghostTimer;
    float ghostCooldownTimer;
    const float GHOST_DURATION = 5.0f;
    const float GHOST_COOLDOWN_MAX = 6.0f;
    // Commit Ghost - fin

    // Commit Crow - debut
    const sf::Texture* defaultPlayerTexture; // Pour identifier l'image de base animée
    bool isAnimated;
    int currentFrame;
    float animationTimer;
    float frameDuration;
    int frameWidth;
    int frameHeight;
    int totalFrames;

    void applyCurrentTexture(const sf::Texture* tex);
    // Commit Crow - fin

    // Commit Pixel-Perfect - debut
    sf::Image normalImage;
    sf::Image ghostImage;
    const sf::Image* currentImage;
    // Commit Pixel-Perfect - fin
    sf::Color skinColor = sf::Color::White;

};