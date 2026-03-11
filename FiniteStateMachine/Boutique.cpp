#include "Boutique.h"
#include <iostream>
Boutique::Boutique(RessourcesManager& rm) :
    itemNameText(rm.getFont(), "", 24),
    priceText(rm.getFont(), "", 20),
    returnButton(rm.getReturnBtnTexture()), 
    background(rm.getMenuBgTexture()),
    buyButton(rm.getBuyBtnTexture()),
    equipButton(rm.getEquipBtnTexture()),
    equippedButton(rm.getEquippedBtnTexture())
{

    handCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
    arrowCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);

    selectedItem = -1;

    font = rm.getFont();

    infoPanel.setSize({ 300.f,800.f });

    itemNameText.setFont(font);
    itemNameText.setCharacterSize(24);
    priceText.setFont(font);
    priceText.setCharacterSize(20);

    itemNameText.setFillColor(sf::Color::White);
    infoPanel.setFillColor(sf::Color(40, 40, 40));
    priceText.setFillColor(sf::Color::Yellow);

    infoPanel.setPosition({ 500.f,0.f });
    returnButton.setPosition({ 20.f,20.f });
    buyButton.setPosition({ 550.f, 400.f });
    itemNameText.setPosition({ 620,50 });
    priceText.setPosition({ 620.f,100.f });
    equipButton.setPosition({ 550.f, 400.f });
    equippedButton.setPosition({ 550.f, 400.f });

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
}

Boutique::Action Boutique::handleClick(sf::Vector2f mousePos)
{

    bool clickedSomething = false;

    // bouton RETURN
    if (returnButton.getGlobalBounds().contains(mousePos))
    {
        return Action::Return;
    }

    // bouton BUY
    if (selectedItem != -1 &&
        !items[selectedItem].isOwned() &&
        buyButton.getGlobalBounds().contains(mousePos))
    {
        items[selectedItem].setOwned(true);
        clickedSomething = true;
    }

    // bouton EQUIP
    if (selectedItem != -1 &&
        items[selectedItem].isOwned() &&
        !items[selectedItem].isEquipped() &&
        equipButton.getGlobalBounds().contains(mousePos))
    {
        for (auto& item : items)
            item.setEquipped(false);

        items[selectedItem].setEquipped(true);

        clickedSomething = true;
    }

    // clic sur un item
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].isClicked(mousePos))
        {
            clickedSomething = true;

            selectedItem = i;

            for (auto& item : items)
                item.setSelected(false);

            items[i].setSelected(true);
        }
    }

    // clic dans le vide
    if (!clickedSomething)
    {
        selectedItem = -1;

        for (auto& item : items)
            item.setSelected(false);
    }

    return Action::None;
}

void Boutique::updateCursor(sf::RenderWindow& window)
{

    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    bool hovering = false;

    if (returnButton.getGlobalBounds().contains(mousePos))
        hovering = true;

    for (auto& item : items)
    {
        if (item.isClicked(mousePos))
        {
            hovering = true;
            break;
        }
    }

    if (hovering && handCursor)
        window.setMouseCursor(*handCursor);
    else if (arrowCursor)
        window.setMouseCursor(*arrowCursor);
}


void Boutique::update(sf::RenderWindow& window)
{

    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto& item : items)
    {
        item.updateHover(mousePos);
    }

    if (selectedItem != -1)
    {

        itemNameText.setString(items[selectedItem].getName());

        priceText.setString(
            "Price : " + std::to_string(items[selectedItem].getPrice())
        );
        previewSprite = sf::Sprite(items[selectedItem].getSprite().getTexture());

        previewSprite->setPosition({ 550.f,200.f });
        previewSprite->setScale({ 3.f,3.f });
        if (items[selectedItem].isOwned())
        {
            buyButton.setColor(sf::Color(150, 150, 150)); // bouton grisé
        }
        else
        {
            buyButton.setColor(sf::Color::White); // bouton normal
        }
    }

}


void Boutique::draw(sf::RenderWindow& window)
{
    window.draw(background);

    for (auto& item : items)
    {
        item.draw(window);
    }
    window.draw(returnButton);


    if (selectedItem != -1)
    {
        window.draw(infoPanel);
        if (previewSprite)
            window.draw(*previewSprite);
        window.draw(itemNameText);
        window.draw(priceText);
        if (!items[selectedItem].isOwned())
        {
            window.draw(buyButton);
        }
        else if (items[selectedItem].isEquipped())
        {
            window.draw(equippedButton);
        }
        else
        {
            window.draw(equipButton);
        }
    }

}