#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "ShopItem.h"
#include "RessourcesManager.h"
#include <optional>
#include "Save.h"

class Boutique
{

private:

    Save& save;

    std::vector<ShopItem> items;

    int selectedItem;

    sf::RectangleShape infoPanel;
    sf::Sprite returnButton;
    sf::Sprite background;
    std::optional<sf::Sprite> previewSprite; // visuel du sprite a droite de la boutique 
    sf::Sprite buyButton;
    sf::Sprite equipButton;
    sf::Sprite equippedButton;

    sf::Font font;
    sf::Text itemNameText;
    sf::Text priceText;
    sf::Text coinsText;


    std::optional<sf::Cursor> handCursor;
    std::optional<sf::Cursor> arrowCursor;

public:

    enum class Action
    {
        None,
        Return
    };

    Boutique(RessourcesManager& rm, Save& save);

    Action handleClick(sf::Vector2f mousePos);

    void update(sf::RenderWindow& window);
    void updateCursor(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

};