#include "Boutique.h"
#include <iostream>
Boutique::Boutique(RessourcesManager& rm, Save& save) :
    save(save),
    itemNameText(rm.getFont(), "", 24),
    priceText(rm.getFont(), "", 20),
    returnButton(rm.getReturnBtnTexture()),
    background(rm.getMenuBgTexture()),
    buyButton(rm.getBuyBtnTexture()),
    equipButton(rm.getEquipBtnTexture()),
    equippedButton(rm.getEquippedBtnTexture()),
    coinsText(rm.getFont())
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
    coinsText.setFont(rm.getFont());
    coinsText.setCharacterSize(24);


    itemNameText.setFillColor(sf::Color::White);
    infoPanel.setFillColor(sf::Color(40, 40, 40));
    priceText.setFillColor(sf::Color::Yellow);
    coinsText.setFillColor(sf::Color::Yellow);


    infoPanel.setPosition({ 500.f,0.f });
    returnButton.setPosition({ 20.f,20.f });
    buyButton.setPosition({ 550.f, 400.f });
    itemNameText.setPosition({ 620,50 });
    priceText.setPosition({ 620.f,100.f });
    equipButton.setPosition({ 550.f, 400.f });
    coinsText.setPosition({ 550.f, 20.f });
    equippedButton.setPosition({ 550.f, 400.f });

    items.emplace_back("Default Bird", 0, rm.getDefaultBirdTexture());
    items.emplace_back("Bird Red", 10, rm.getBirdRedTexture());
    items.emplace_back("Bird Blue", 250, rm.getBirdBlueTexture());
    items.emplace_back("Bird Green", 350, rm.getBirdGreenTexture());
    items.emplace_back("Bird Gold", 550, rm.getBirdGoldTexture());
    items.emplace_back("Bird Shadow", 850, rm.getBirdShadowTexture());

    int cols = 3;
    int spacing = 20;

    for (int i = 0; i < items.size(); i++)
    {
        if (save.isSkinOwned(i))
            items[i].setOwned(true);

        if (save.getEquippedSkin() == i)
            items[i].setEquipped(true);

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
        int price = items[selectedItem].getPrice();
        int coins = save.getTotalScore();
        if (coins >= price)
        {
            save.spendCoins(price);
            save.buySkin(selectedItem);
            items[selectedItem].setOwned(true);
        }
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

        save.equipSkin(selectedItem);
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

    coinsText.setString("Coins : " + std::to_string(save.getTotalScore()));
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
        previewSprite->setScale({ 0.04f,0.04f });
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
    window.draw(coinsText);

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