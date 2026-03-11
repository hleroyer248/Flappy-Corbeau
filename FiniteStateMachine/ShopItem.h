#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ShopItem
{

private:

    std::string name;
    int price;

    bool owned;
    bool selected;
    bool equipped;
 

    sf::Sprite sprite;
    sf::RectangleShape box;

public:

    ShopItem(const std::string& name, int price, const sf::Texture& texture);

    void setPosition(float x, float y);

    void draw(sf::RenderWindow& window);

    bool isClicked(sf::Vector2f mousePos) const;

    std::string getName() const;
    int getPrice() const;

    bool isOwned() const;
    void setOwned(bool value);
    void updateHover(sf::Vector2f mousePos);
    void setSelected(bool value);

    const sf::Sprite& getSprite() const;

    void setEquipped(bool value);
    bool isEquipped() const;

};