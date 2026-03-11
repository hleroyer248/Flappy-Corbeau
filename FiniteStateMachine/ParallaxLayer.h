#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
public:
    ParallaxLayer(const sf::Texture& tex, float speedFactor)
        : factor(speedFactor) {
        // On initialise les deux sprites avec la même texture
        s1.setTexture(tex);
        s2.setTexture(tex);

        // On récupère la largeur pour le positionnement
        width = static_cast<float>(tex.getSize().x);

        s1.setPosition({ 0.f, 0.f });
        s2.setPosition({ width, 0.f });
    }

    void update(float dt, float baseSpeed) {
        float speed = baseSpeed * factor;
        s1.move({ -speed * dt, 0.f });
        s2.move({ -speed * dt, 0.f });

        // Si un sprite sort de l'écran, on le replace derrière l'autre
        if (s1.getPosition().x <= -width) {
            s1.setPosition({ s2.getPosition().x + width, 0.f });
        }
        if (s2.getPosition().x <= -width) {
            s2.setPosition({ s1.getPosition().x + width, 0.f });
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(s1);
        window.draw(s2);
    }

    // Pour que le dash affecte aussi le décor
    void shift(float distance) {
        s1.move({ -distance * factor, 0.f });
        s2.move({ -distance * factor, 0.f });
    }

private:
    sf::Sprite s1, s2;
    float factor;
    float width;
};

