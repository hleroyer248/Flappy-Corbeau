#pragma once
#include <SFML/Graphics.hpp>

class CollisionBox {
public:
    CollisionBox();
    CollisionBox(const sf::FloatRect& rect);

    // Commit Pixel-Perfect - debut
    CollisionBox(const sf::Sprite& sprite, const sf::Image& image);
    // Commit Pixel-Perfect - fin

    void setRect(const sf::FloatRect& rect);
    const sf::FloatRect& getRect() const;
    bool intersects(const CollisionBox& other) const;

private:
    sf::FloatRect bounds;

    // Commit Pixel-Perfect - debut
    const sf::Sprite* sprite;
    const sf::Image* image;
    // Commit Pixel-Perfect - fin
};