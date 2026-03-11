#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "ShopItem.h"
#include "RessourcesManager.h"
#include <optional>

class Boutique
{

private:

    std::vector<ShopItem> items;

    int selectedItem;

    sf::RectangleShape infoPanel;
    sf::Sprite returnButton;
    sf::Sprite background;
    std::optional<sf::Sprite> previewSprite; // visuel du sprite a droite de la boutique 
    sf::Sprite buyButton;
    sf::Sprite equipButton;

    sf::Font font;
    sf::Text itemNameText;
    sf::Text priceText;


    std::optional<sf::Cursor> handCursor;
    std::optional<sf::Cursor> arrowCursor;

public:

    enum class Action
    {
        None,
        Return
    };

    Boutique(RessourcesManager& rm);

    Action handleClick(sf::Vector2f mousePos);

    void update(sf::RenderWindow& window);
    void updateCursor(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

};