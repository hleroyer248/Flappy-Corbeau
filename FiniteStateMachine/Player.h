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
    const sf::Vector2f& getPosition() const;

    void setSkin(const sf::Texture& texture);

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
    const float GHOST_COOLDOWN_MAX = 1.0f;
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

    // Méthode utilitaire pour appliquer correctement l'animation et le scale
    void applyCurrentTexture(const sf::Texture* tex);
    // Commit Crow - fin
};