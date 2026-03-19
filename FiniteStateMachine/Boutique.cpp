#include "Boutique.h"
#include <iostream>


Boutique::Boutique(RessourcesManager& rm, Save& save) :
    save(save),
    itemNameText(rm.getFont(), "", 40),
    priceText(rm.getFont(), "", 30),
    returnButton(rm.getReturnBtnTexture()),
    background(rm.getMenuBgTexture()),
    buyButton(rm.getBuyBtnTexture()),
    equipButton(rm.getEquipBtnTexture()),
    equippedButton(rm.getEquippedBtnTexture()),
    coinsText(rm.getFont()),
    warningText(rm.getFont(), "not enough to cover the purchase", 22) 
{
    float scaleX = 1920.f / rm.getMenuBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getMenuBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    handCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
    arrowCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);

    selectedItem = -1;
    font = rm.getFont();

    infoPanel.setSize({ 600.f, 1080.f });
    infoPanel.setFillColor(sf::Color(40, 40, 40));
    infoPanel.setPosition({ 1320.f, 0.f });

    itemNameText.setFillColor(sf::Color::White);
    priceText.setFillColor(sf::Color::Yellow);
    coinsText.setFillColor(sf::Color::Yellow);
    coinsText.setCharacterSize(40);

    warningText.setFillColor(sf::Color::Red);
    sf::FloatRect textBounds = warningText.getLocalBounds();
    warningText.setOrigin({ textBounds.size.x / 2.0f, 0.f });
    warningText.setPosition({ 1620.f, 660.f });

    returnButton.setPosition({ 10.f, 20.f });
    returnButton.setScale({ 0.09f, 0.09f });

    buyButton.setPosition({ 1420.f, 700.f });
    equipButton.setPosition({ 1420.f, 700.f });
    equippedButton.setPosition({ 1420.f, 700.f });

    auto forceButtonWidth = [](sf::Sprite& sprite, float targetWidth) {
        float originalWidth = sprite.getLocalBounds().size.x;
        if (originalWidth > 0.f) {
            float scale = targetWidth / originalWidth;
            sprite.setScale({ scale, scale });
        }
        };

    forceButtonWidth(buyButton, 400.f);
    forceButtonWidth(equipButton, 400.f);
    forceButtonWidth(equippedButton, 400.f);

    itemNameText.setPosition({ 1450.f, 100.f });
    priceText.setPosition({ 1450.f, 180.f });
    coinsText.setPosition({ 350.f, 40.f });

    items.emplace_back("Bird Default", 0, rm.getPlayerTexture());
    items.emplace_back("Bird Red", 10, rm.getPlayerTexture());
    items.emplace_back("Bird Blue", 250, rm.getPlayerTexture());
    items.emplace_back("Bird Green", 350, rm.getPlayerTexture());
    items.emplace_back("Bird Gold", 550, rm.getPlayerTexture());
    items.emplace_back("Bird Shadow", 850, rm.getPlayerTexture());
    items.emplace_back("Bird Rainbow", 3000, rm.getPlayerTexture());

    items[0].setColor(sf::Color::White);
    items[1].setColor(sf::Color::Red);
    items[2].setColor(sf::Color::Blue);
    items[3].setColor(sf::Color::Green);
    items[4].setColor(sf::Color(255, 215, 0)); 
    items[5].setColor(sf::Color(100, 100, 100)); 


    int cols = 3;
    int spacing = 40;

    for (int i = 0; i < items.size(); i++)
    {
        if (save.isSkinOwned(i)) items[i].setOwned(true);
        if (save.getEquippedSkin() == i || (save.getEquippedSkin() == -1 && i == 0))
            items[i].setEquipped(true);

        int x = i % cols;
        int y = i / cols;

        items[i].setPosition(
            150.f + x * (200.f + spacing),
            200.f + y * (200.f + spacing)
        );
    }
}

Boutique::Action Boutique::handleClick(sf::Vector2f mousePos) {
    bool clickedSomething = false;

    if (returnButton.getGlobalBounds().contains(mousePos)) return Action::Return;

    if (selectedItem != -1 && !items[selectedItem].isOwned() && buyButton.getGlobalBounds().contains(mousePos)) {
        int price = items[selectedItem].getPrice();
        int coins = save.getTotalScore();
        if (coins >= price) {
            save.spendCoins(price);
            save.buySkin(selectedItem);
            items[selectedItem].setOwned(true);
            showWarning = false; 
        }
        else {
            showWarning = true;
            warningClock.restart();
        }
        clickedSomething = true;
    }

    if (selectedItem != -1 && items[selectedItem].isOwned() && !items[selectedItem].isEquipped() && equipButton.getGlobalBounds().contains(mousePos)) {
        for (auto& item : items) item.setEquipped(false);
        save.equipSkin(selectedItem);
        items[selectedItem].setEquipped(true);
        clickedSomething = true;
    }

    for (int i = 0; i < items.size(); i++) {
        if (items[i].isClicked(mousePos)) {
            clickedSomething = true;

            if (selectedItem != i) {
                showWarning = false;
            }

            selectedItem = i;
            for (auto& item : items) item.setSelected(false);
            items[i].setSelected(true);
        }
    }

    if (!clickedSomething) {
        selectedItem = -1;
        for (auto& item : items) item.setSelected(false);
    }

    return Action::None;
}

void Boutique::updateCursor(sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool hovering = false;

    if (returnButton.getGlobalBounds().contains(mousePos)) hovering = true;

    for (auto& item : items) {
        if (item.isClicked(mousePos)) {
            hovering = true;
            break;
        }
    }

    if (hovering && handCursor) window.setMouseCursor(*handCursor);
    else if (arrowCursor) window.setMouseCursor(*arrowCursor);
}


void Boutique::update(sf::RenderWindow& window) {
    coinsText.setString("Coins : " + std::to_string(save.getTotalScore()));
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto& item : items) item.updateHover(mousePos);
    float time = clock.getElapsedTime().asSeconds();

    if (showWarning && warningClock.getElapsedTime().asSeconds() >= 3.0f) {
        showWarning = false;
    }

    for (int i = 0; i < items.size(); i++) {
        if (i == 6) { 
            items[i].setColor(getRainbowColor(time));
        }
    }
    if (selectedItem != -1) {
        itemNameText.setString(items[selectedItem].getName());
        priceText.setString("Price : " + std::to_string(items[selectedItem].getPrice()));

        previewSprite = items[selectedItem].getSprite();

        auto bounds = previewSprite->getLocalBounds();
        if (selectedItem == 6) {
            previewSprite->setColor(getRainbowColor(time));
        }

        float targetSize = 250.f; 
        float scale = targetSize / bounds.size.x;

        previewSprite->setScale({ scale, scale });
        previewSprite->setPosition({ 1600.f, 400.f });

        if (items[selectedItem].isOwned()) buyButton.setColor(sf::Color(150, 150, 150));
        else buyButton.setColor(sf::Color::White);
    }
}

void Boutique::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(coinsText);

    for (auto& item : items) item.draw(window);

    window.draw(returnButton);

    if (selectedItem != -1) {
        window.draw(infoPanel);
        window.draw(*previewSprite);
        window.draw(itemNameText);
        window.draw(priceText);

        if (!items[selectedItem].isOwned()) {
            window.draw(buyButton);

            if (showWarning) {
                window.draw(warningText);
            }
        }
        else if (items[selectedItem].isEquipped()) window.draw(equippedButton);
        else window.draw(equipButton);
    }
}
