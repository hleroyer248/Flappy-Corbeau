#include "Boutique.h"
#include <iostream>
Boutique::Boutique(RessourcesManager& rm):
itemNameText(rm.getFont(), "", 24),
priceText(rm.getFont(), "", 20),
returnButton(rm.getReturnBtnTexture())
{

    selectedItem = -1;

    font = rm.getFont();

    itemNameText.setFont(font);
    itemNameText.setCharacterSize(24);
    itemNameText.setFillColor(sf::Color::White);
    itemNameText.setPosition({ 620,50 });

    priceText.setFont(font);
    priceText.setCharacterSize(20);
    priceText.setFillColor(sf::Color::Yellow);
    priceText.setPosition({ 620.f,100.f });

    items.emplace_back("Bird Red", 150, rm.getPlayerTexture());
    items.emplace_back("Bird Blue", 250, rm.getPlayerTexture());
    items.emplace_back("Bird Green", 350, rm.getPlayerTexture());
    items.emplace_back("Bird Gold", 550, rm.getPlayerTexture());
    items.emplace_back("Bird Shadow", 850, rm.getPlayerTexture());

    int cols = 3;
    int spacing = 20;

    for (int i = 0; i < items.size(); i++)
    {

        int x = i % cols;
        int y = i / cols;

        items[i].setPosition(
            50 + x * (120 + spacing),
            100 + y * (120 + spacing)
        );

    }

    infoPanel.setSize({ 300.f,800.f });
    infoPanel.setPosition({ 500.f,0.f });
    infoPanel.setFillColor(sf::Color(40, 40, 40));

    returnButton.setPosition({20.f,20.f});

}

Boutique::Action Boutique::handleClick(sf::Vector2f mousePos)
{

    if (returnButton.getGlobalBounds().contains(mousePos))
    {
        return Action::Return;
    }

    for (int i = 0; i < items.size(); i++)
    {

        if (items[i].isClicked(mousePos))
        {
            selectedItem = i;
        }

    }

    return Action::None;
}

void Boutique::update()
{

    if (selectedItem != -1)
    {

        itemNameText.setString(items[selectedItem].getName());

        priceText.setString(
            "Price : " + std::to_string(items[selectedItem].getPrice())
        );

    }

}

void Boutique::draw(sf::RenderWindow& window)
{

    for (auto& item : items)
    {
        item.draw(window);
    }
    window.draw(returnButton);


    if (selectedItem != -1)
    {
        window.draw(infoPanel);
        window.draw(itemNameText);
        window.draw(priceText);
    }

}