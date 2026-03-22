#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& tex, float speedFactor, sf::IntRect rect)
    : factor(speedFactor), s1(tex), s2(tex)
{
    int height;
    int croppedWidth;

    if (rect.size.x > 0 && rect.size.y > 0) {
        s1.setTextureRect(rect);
        s2.setTextureRect(rect);
        float scaleX = 1.1f;

        s1.setScale({ scaleX, 1.f });
        s2.setScale({ scaleX, 1.f });

        croppedWidth = rect.size.x;
        height = rect.size.y;

        width = static_cast<float>(croppedWidth) * scaleX;
    }
    else {
        int cropLeft = 43;
        int cropRight = 40;

        croppedWidth = tex.getSize().x - cropLeft - cropRight;
        height = tex.getSize().y;

        sf::IntRect cropRect({ cropLeft, 0 }, { croppedWidth, height });

        s1.setTextureRect(cropRect);
        s2.setTextureRect(cropRect);

        float scaleY = 1080.f / static_cast<float>(height); // ✅ déplacé ici
        float scaleX = scaleY;

        s1.setScale({ scaleX, scaleY });
        s2.setScale({ scaleX, scaleY });

        width = static_cast<float>(croppedWidth) * scaleX;
    }

    s1.setPosition({ 0.f, 0.f });
    s2.setPosition({ width, 0.f });
}

void ParallaxLayer::update(float dt, float baseSpeed)
{
    float speed = baseSpeed * factor;
    s1.move({ -speed * dt, 0.f });
    s2.move({ -speed * dt, 0.f });


    if (s1.getPosition().x + width <= 0) {
        s1.setPosition({ s1.getPosition().x + 2 * width, s1.getPosition().y });
    }

    if (s2.getPosition().x + width <= 0) {
        s2.setPosition({ s2.getPosition().x + 2 * width, s2.getPosition().y });
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window) const {
    window.draw(s1);
    window.draw(s2);
}

void ParallaxLayer::setY(float y) {
    s1.setPosition({ s1.getPosition().x, y });
    s2.setPosition({ s2.getPosition().x, y });
}