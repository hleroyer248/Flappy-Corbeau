#pragma once
#include <SFML/Graphics.hpp>

class CollisionBox {
public:
    CollisionBox();
    CollisionBox(const sf::FloatRect& rect);
    void setRect(const sf::FloatRect& rect);
    const sf::FloatRect& getRect() const;
    bool intersects(const CollisionBox& other) const;

private:
    sf::FloatRect bounds;
};