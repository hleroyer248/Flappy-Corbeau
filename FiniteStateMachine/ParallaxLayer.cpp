#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& tex, float speedFactor)
    : factor(speedFactor), s1(tex), s2(tex)
{
    int cropLeft = 43;
    int cropRight = 40;

    int croppedWidth = tex.getSize().x - cropLeft - cropRight;
    int height = tex.getSize().y;

    sf::IntRect cropRect({ cropLeft, 0 }, { croppedWidth, height });

    s1.setTextureRect(cropRect);
    s2.setTextureRect(cropRect);

    float scaleY = 1080.f / static_cast<float>(height);
    float scaleX = scaleY;

    s1.setScale({ scaleX, scaleY });
    s2.setScale({ scaleX, scaleY });

    width = static_cast<float>(croppedWidth) * scaleX;

    s1.setPosition({ 0.f, 0.f });

    s2.setPosition({ width - 1.f, 0.f });
}

void ParallaxLayer::update(float dt, float baseSpeed)
{
    float speed = baseSpeed * factor;
    s1.move({ -speed * dt, 0.f });
    s2.move({ -speed * dt, 0.f });

    float raccord = width - 1.f;

    if (s1.getPosition().x <= -raccord) {
        s1.setPosition({ s2.getPosition().x + raccord, s1.getPosition().y });
    }

    if (s2.getPosition().x <= -raccord) {
        s2.setPosition({ s1.getPosition().x + raccord, s2.getPosition().y });
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window) const {
    window.draw(s1);
    window.draw(s2);
}
