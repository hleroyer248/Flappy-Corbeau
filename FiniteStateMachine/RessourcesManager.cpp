#include "RessourcesManager.h"
#include <iostream>

RessourcesManager::RessourcesManager() {}

bool RessourcesManager::loadAll() {
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "ERREUR: Impossible de charger arial.ttf.\n";
        return false;
    }
    if (!playerTex.loadFromFile("Player.png") ||
        !bgTex.loadFromFile("Background.png") ||
        !topPipeTex.loadFromFile("Hobstacle.png") ||
        !bottomPipeTex.loadFromFile("Bobstacle.png") ||
        !menuBgTex.loadFromFile("MenuBackground.png") ||
        !startBtnTex.loadFromFile("StartButton.png") ||
        !settingsBtnTex.loadFromFile("SettingsButton.png") ||
        !exitBtnTex.loadFromFile("ExitButton.png") ||
        !settingsBgTex.loadFromFile("SettingsBackground.png") ||
        !returnBtnTex.loadFromFile("ReturnButton.png")) {
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