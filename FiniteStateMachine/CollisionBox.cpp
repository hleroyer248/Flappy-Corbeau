#include "CollisionBox.h"

CollisionBox::CollisionBox() {}

CollisionBox::CollisionBox(const sf::FloatRect& rect) : bounds(rect) {}

void CollisionBox::setRect(const sf::FloatRect& rect) {
    bounds = rect;
}

const sf::FloatRect& CollisionBox::getRect() const {
    return bounds;
}

bool CollisionBox::intersects(const CollisionBox& other) const {
    return bounds.findIntersection(other.getRect()).has_value();
}