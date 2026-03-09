#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ShopItem
{

private:

    std::string name;
    int price;

    bool owned;
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

};