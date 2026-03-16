#include "CollisionBox.h"

CollisionBox::CollisionBox() : sprite(nullptr), image(nullptr) {}

CollisionBox::CollisionBox(const sf::FloatRect& rect) : bounds(rect), sprite(nullptr), image(nullptr) {}

// Commit Pixel-Perfect - debut
CollisionBox::CollisionBox(const sf::Sprite& s, const sf::Image& img)
    : bounds(s.getGlobalBounds()), sprite(&s), image(&img) {
}
// Commit Pixel-Perfect - fin

void CollisionBox::setRect(const sf::FloatRect& rect) {
    bounds = rect;
}

const sf::FloatRect& CollisionBox::getRect() const {
    return bounds;
}

bool CollisionBox::intersects(const CollisionBox& other) const {
    // 1. Vérification basique AABB (boîte englobante)
    std::optional<sf::FloatRect> intersection = bounds.findIntersection(other.getRect());

    if (!intersection) {
        return false;
    }

    // Commit Pixel-Perfect - debut
    // 2. Si on n'a pas les images pour le Pixel-Perfect, on se contente du AABB
    if (!sprite || !image || !other.sprite || !other.image) {
        return true;
    }

    // 3. Vérification Pixel-Perfect dans la zone d'intersection
    sf::FloatRect intersect = intersection.value();

    int left = static_cast<int>(intersect.position.x);
    int top = static_cast<int>(intersect.position.y);
    int right = static_cast<int>(intersect.position.x + intersect.size.x);
    int bottom = static_cast<int>(intersect.position.y + intersect.size.y);

    for (int x = left; x < right; ++x) {
        for (int y = top; y < bottom; ++y) {

            sf::Vector2f local1 = sprite->getInverseTransform().transformPoint({ static_cast<float>(x), static_cast<float>(y) });
            int texX1 = static_cast<int>(local1.x) + sprite->getTextureRect().position.x;
            int texY1 = static_cast<int>(local1.y) + sprite->getTextureRect().position.y;

            sf::Vector2f local2 = other.sprite->getInverseTransform().transformPoint({ static_cast<float>(x), static_cast<float>(y) });
            int texX2 = static_cast<int>(local2.x) + other.sprite->getTextureRect().position.x;
            int texY2 = static_cast<int>(local2.y) + other.sprite->getTextureRect().position.y;

            if (texX1 >= 0 && texX1 < static_cast<int>(image->getSize().x) && texY1 >= 0 && texY1 < static_cast<int>(image->getSize().y) &&
                texX2 >= 0 && texX2 < static_cast<int>(other.image->getSize().x) && texY2 >= 0 && texY2 < static_cast<int>(other.image->getSize().y)) {

                if (image->getPixel({ static_cast<unsigned int>(texX1), static_cast<unsigned int>(texY1) }).a > 0 &&
                    other.image->getPixel({ static_cast<unsigned int>(texX2), static_cast<unsigned int>(texY2) }).a > 0) {
                    return true;
                }
            }
        }
    }
    // Commit Pixel-Perfect - fin

    return false;
}

void CollisionBox::debugDraw(sf::RenderWindow& window) const {
    sf::RectangleShape shape;
    shape.setPosition(bounds.position);
    shape.setSize(bounds.size);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(2.f);
    window.draw(shape);
}