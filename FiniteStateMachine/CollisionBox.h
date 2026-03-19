#pragma once
#include <SFML/Graphics.hpp>

class CollisionBox {
public:
    CollisionBox();
    CollisionBox(const sf::FloatRect& rect);
    CollisionBox(const sf::Sprite& sprite, const sf::Image& image);

    void setRect(const sf::FloatRect& rect);
    const sf::FloatRect& getRect() const;
    bool intersects(const CollisionBox& other) const;
    void debugDraw(sf::RenderWindow& window) const;

private:
    sf::FloatRect bounds;

    const sf::Sprite* sprite;
    const sf::Image* image;
};
