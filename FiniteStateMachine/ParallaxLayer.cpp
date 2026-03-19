#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& tex, float speedFactor)
    : factor(speedFactor), s1(tex), s2(tex)
{
    float scaleY = 1080.f * 0.928f / static_cast<float>(tex.getSize().y);

    // On applique le même scale sur X pour garder les proportions de l'image
    float scaleX = scaleY;

    s1.setScale({ scaleX, scaleY });
    s2.setScale({ scaleX, scaleY });

    // On calcule la largeur réelle du sprite redimensionné pour la boucle
    width = static_cast<float>(tex.getSize().x) * scaleX;

    // Positionnement initial (0,0) car on remplit tout l'écran
    s1.setPosition({ 0.f, 0.f });
    s2.setPosition({ width, 0.f });
}

void ParallaxLayer::update(float dt, float baseSpeed)
{
    float speed = baseSpeed * factor;
    s1.move({ -speed * dt, 0.f });
    s2.move({ -speed * dt, 0.f });

    if (s1.getPosition().x <= -width) {
        s1.setPosition({ s2.getPosition().x + width - 1.f, 0.f });
    }
    if (s2.getPosition().x <= -width) {
        s2.setPosition({ s1.getPosition().x + width - 1.f, 0.f });
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window) const {
    window.draw(s1);
    window.draw(s2);
}