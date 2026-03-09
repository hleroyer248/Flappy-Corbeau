#include "ShopItem.h"

ShopItem::ShopItem(const std::string& name, int price, const sf::Texture& texture):
sprite(texture)
{

    this->name = name;
    this->price = price;

    owned = false;
    equipped = false;

    sprite.setTexture(texture);
    sprite.setScale({ 2.f, 2.f });

    box.setSize({ 120.f, 120.f });
    box.setFillColor(sf::Color(70, 70, 70));

}

void ShopItem::setPosition(float x, float y)
{

    box.setPosition({ x,y });
    sprite.setPosition({ x + 20,y + 20 });

}

void ShopItem::draw(sf::RenderWindow& window)
{

    window.draw(box);
    window.draw(sprite);

}

bool ShopItem::isClicked(sf::Vector2f mousePos) const
{

    return box.getGlobalBounds().contains(mousePos);

}

std::string ShopItem::getName() const
{
    return name;
}

int ShopItem::getPrice() const
{
    return price;
}

bool ShopItem::isOwned() const
{
    return owned;
}

void ShopItem::setOwned(bool value)
{
    owned = value;
}