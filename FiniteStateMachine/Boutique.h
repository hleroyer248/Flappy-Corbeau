#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "ShopItem.h"
#include "RessourcesManager.h"

class Boutique
{

private:

    std::vector<ShopItem> items;

    int selectedItem;

    sf::RectangleShape infoPanel;

    sf::Font font;
    sf::Text itemNameText;
    sf::Text priceText;

public:

    Boutique(RessourcesManager& rm);

    void handleClick(sf::Vector2f mousePos);

    void update();

    void draw(sf::RenderWindow& window);

};