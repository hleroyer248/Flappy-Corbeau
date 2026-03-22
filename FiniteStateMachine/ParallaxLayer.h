#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
public:
    ParallaxLayer(const sf::Texture& tex, float speedFactor, sf::IntRect rect = sf::IntRect());

    void update(float dt, float baseSpeed);
    void draw(sf::RenderWindow& window) const;
    void setY(float y);

private:
    sf::Sprite s1;
    sf::Sprite s2;
    float factor;
    float width;
};