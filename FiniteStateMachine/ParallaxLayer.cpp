#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& tex, float speedFactor)
    : factor(speedFactor),
    s1(tex),
    s2(tex)
{
    // On étire la texture pour qu'elle remplisse parfaitement le 1920x1080
    float scaleX = 1920.f / tex.getSize().x;
    float scaleY = 1080.f / tex.getSize().y;

    s1.setScale({ scaleX, scaleY });
    s2.setScale({ scaleX, scaleY });

    width = tex.getSize().x * scaleX;

    s1.setPosition({ 0.f, 0.f });
    s2.setPosition({ width, 0.f });
}

void ParallaxLayer::update(float dt, float baseSpeed)
{
    float speed = baseSpeed * factor;
    s1.move({ -speed * dt, 0.f });
    s2.move({ -speed * dt, 0.f });

    if (s1.getPosition().x <= -width) {
        s1.setPosition({ s2.getPosition().x + width, 0.f });
    }
    if (s2.getPosition().x <= -width) {
        s2.setPosition({ s1.getPosition().x + width, 0.f });
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window) const {
    window.draw(s1);
    window.draw(s2);
}