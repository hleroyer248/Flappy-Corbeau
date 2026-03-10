#include "RessourcesManager.h"
#include <iostream>

RessourcesManager::RessourcesManager() {}

bool RessourcesManager::loadAll() {
    const std::string basePath = "Assets/";

    if (!font.openFromFile(basePath + "arial.ttf")) {
        std::cerr << "ERREUR: Impossible de charger arial.ttf depuis le dossier Assets.\n";
        return false;
    }
    if (!playerTex.loadFromFile(basePath + "Player.png") ||
        !bgTex.loadFromFile(basePath + "Background.png") ||
        !topPipeTex.loadFromFile(basePath + "Hobstacle.png") ||
        !bottomPipeTex.loadFromFile(basePath + "Bobstacle.png") ||
        !menuBgTex.loadFromFile(basePath + "MenuBackground.png") ||
        !startBtnTex.loadFromFile(basePath + "StartButton.png") ||
        !settingsBtnTex.loadFromFile(basePath + "SettingsButton.png") ||
        !exitBtnTex.loadFromFile(basePath + "ExitButton.png") ||
        !settingsBgTex.loadFromFile(basePath + "SettingsBackground.png") ||
        !returnBtnTex.loadFromFile(basePath + "ReturnButton.png") ||
        !shopTex.loadFromFile(basePath + "UIShop.jpg") ||
        !buyBtnTex.loadFromFile(basePath + "BuyButton.jpg")) {
        std::cerr << "ERREUR: Impossible de charger un ou plusieurs assets png.\n";
        return false;
    }
    return true;
}

const sf::Texture& RessourcesManager::getPlayerTexture() const { return playerTex; }
const sf::Texture& RessourcesManager::getBgTexture() const { return bgTex; }
const sf::Texture& RessourcesManager::getTopPipeTexture() const { return topPipeTex; }
const sf::Texture& RessourcesManager::getBottomPipeTexture() const { return bottomPipeTex; }
const sf::Font& RessourcesManager::getFont() const { return font; }

const sf::Texture& RessourcesManager::getMenuBgTexture() const { return menuBgTex; }
const sf::Texture& RessourcesManager::getStartBtnTexture() const { return startBtnTex; }
const sf::Texture& RessourcesManager::getSettingsBtnTexture() const { return settingsBtnTex; }
const sf::Texture& RessourcesManager::getExitBtnTexture() const { return exitBtnTex; }
const sf::Texture& RessourcesManager::getSettingsBgTexture() const { return settingsBgTex; }
const sf::Texture& RessourcesManager::getReturnBtnTexture() const { return returnBtnTex; }
const sf::Texture& RessourcesManager::getShopTexture() const{ return shopTex; }
const sf::Texture& RessourcesManager::getBuyBtnTexture() const { return buyBtnTex; }