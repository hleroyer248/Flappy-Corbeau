#include "ShopItem.h"

ShopItem::ShopItem(const std::string& name, int price, const sf::Texture& texture) :
    sprite(texture), selected(false)
{

    this->name = name;
    this->price = price;

    owned = false;
    equipped = false;

    auto texSize = texture.getSize();

    // calcul automatique d'une frame
    int frameWidth = texSize.x / 3;
    int frameHeight = texSize.y / 3;

    sprite.setTextureRect(sf::IntRect({ 0,0 }, { frameWidth,frameHeight }));

    // adapter le scale pour rentrer dans la box
    float targetSize = 120.f;
    float scale = targetSize / frameWidth;

    sprite.setScale({ scale, scale });
    box.setSize({ 200.f, 200.f });
    box.setFillColor(sf::Color(70, 70, 70));

}

void ShopItem::setPosition(float x, float y)
{

    box.setPosition({ x,y });

    sprite.setOrigin({
        sprite.getLocalBounds().size.x / 2.f,
        sprite.getLocalBounds().size.y / 2.f
        });

    sprite.setPosition({
        x + box.getSize().x / 2.f,
        y + box.getSize().y / 2.f
        });

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