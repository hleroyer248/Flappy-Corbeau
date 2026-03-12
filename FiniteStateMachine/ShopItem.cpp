#include "ShopItem.h"

ShopItem::ShopItem(const std::string& name, int price, const sf::Texture& texture):
sprite(texture), selected(false)
{

    this->name = name;
    this->price = price;

    owned = false;
    equipped = false;

    sprite.setTexture(texture);
    sprite.setScale({ 0.04f, 0.04f });

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
    if (equipped)
    {
        box.setOutlineColor(sf::Color::Green);
        box.setOutlineThickness(4.f);
        box.setFillColor(sf::Color(90, 90, 90));
    }
    else
    {
        box.setOutlineThickness(0.f);
    }
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

void ShopItem::updateHover(sf::Vector2f mousePos)
{

    if (selected)
    {
        box.setFillColor(sf::Color(200, 200, 50)); // jaune
        return;
    }

    if (box.getGlobalBounds().contains(mousePos))
    {
        box.setFillColor(sf::Color(120, 120, 120)); // hover
    }
    else
    {
        box.setFillColor(sf::Color(70, 70, 70)); // normal
    }

}

void ShopItem::setSelected(bool value)
{
    selected = value;
}

const sf::Sprite& ShopItem::getSprite() const
{
    return sprite;
}

void ShopItem::setEquipped(bool value)
{
    equipped = value;
}

bool ShopItem::isEquipped() const
{
    return equipped;
}

const sf::Texture& ShopItem::getTexture() const
{
    return sprite.getTexture();
}