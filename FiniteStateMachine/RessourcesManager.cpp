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
        !ghostPlayerTex.loadFromFile(basePath + "Ghost-Player.png") || // Commit Ghost
        !bgTex.loadFromFile(basePath + "Background.png") ||
        !midBgTex.loadFromFile(basePath + "MidBackground.png") ||      // Commit Parallax
        !frontBgTex.loadFromFile(basePath + "FrontBackground.png") ||  // Commit Parallax
        !topPipeTex.loadFromFile(basePath + "Hobstacle.png") ||
        !bottomPipeTex.loadFromFile(basePath + "Bobstacle.png") ||
        !menuBgTex.loadFromFile(basePath + "MenuBackground.png") ||
        !startBtnTex.loadFromFile(basePath + "StartButton.png") ||
        !settingsBtnTex.loadFromFile(basePath + "SettingsButton.png") ||
        !exitBtnTex.loadFromFile(basePath + "ExitButton.png") ||
        !settingsBgTex.loadFromFile(basePath + "SettingsBackground.png") ||
        !returnBtnTex.loadFromFile(basePath + "ReturnButton.png") ||
        !shopTex.loadFromFile(basePath + "ShopButton.png") ||
        !buyBtnTex.loadFromFile(basePath + "BuyButton.jpg") ||
        !equipBtnTex.loadFromFile(basePath + "EquipButton.png") ||
        !equippedBtnTex.loadFromFile(basePath + "EquippedButton.png") ||
        !birdRedTex.loadFromFile(basePath + "BirdRed.png") ||
        !birdBlueTex.loadFromFile(basePath + "BirdBlue.png") ||
        !birdGreenTex.loadFromFile(basePath + "BirdGreen.png") ||
        !birdGoldTex.loadFromFile(basePath + "BirdGold.png") ||
        !birdShadowTex.loadFromFile(basePath + "BirdShadow.png")) {
        std::cerr << "ERREUR: Impossible de charger un ou plusieurs assets png.\n";
        return false;
    }

    // Commit Pixel-Perfect - debut
    topPipeImg = topPipeTex.copyToImage();
    bottomPipeImg = bottomPipeTex.copyToImage();
    // Commit Pixel-Perfect - fin

    return true;
}

const sf::Texture& RessourcesManager::getPlayerTexture() const { return playerTex; }
const sf::Texture& RessourcesManager::getBgTexture() const { return bgTex; }

// Commit Parallax - debut
const sf::Texture& RessourcesManager::getMidBgTexture() const { return midBgTex; }
const sf::Texture& RessourcesManager::getFrontBgTexture() const { return frontBgTex; }
// Commit Parallax - fin

const sf::Texture& RessourcesManager::getTopPipeTexture() const { return topPipeTex; }
const sf::Texture& RessourcesManager::getBottomPipeTexture() const { return bottomPipeTex; }
const sf::Font& RessourcesManager::getFont() const { return font; }

const sf::Texture& RessourcesManager::getMenuBgTexture() const { return menuBgTex; }
const sf::Texture& RessourcesManager::getStartBtnTexture() const { return startBtnTex; }
const sf::Texture& RessourcesManager::getSettingsBtnTexture() const { return settingsBtnTex; }
const sf::Texture& RessourcesManager::getExitBtnTexture() const { return exitBtnTex; }
const sf::Texture& RessourcesManager::getSettingsBgTexture() const { return settingsBgTex; }
const sf::Texture& RessourcesManager::getReturnBtnTexture() const { return returnBtnTex; }
const sf::Texture& RessourcesManager::getShopTexture() const { return shopTex; }
const sf::Texture& RessourcesManager::getBuyBtnTexture() const { return buyBtnTex; }
const sf::Texture& RessourcesManager::getEquipBtnTexture() const { return equipBtnTex; }
const sf::Texture& RessourcesManager::getEquippedBtnTexture() const { return equippedBtnTex; }
const sf::Texture& RessourcesManager::getGhostPlayerTexture() const { return ghostPlayerTex; } // Commit Ghost

const sf::Texture& RessourcesManager::getBirdRedTexture() const { return birdRedTex; }
const sf::Texture& RessourcesManager::getBirdBlueTexture() const { return birdBlueTex; }
const sf::Texture& RessourcesManager::getBirdGreenTexture() const { return birdGreenTex; }
const sf::Texture& RessourcesManager::getBirdGoldTexture() const { return birdGoldTex; }
const sf::Texture& RessourcesManager::getBirdShadowTexture() const { return birdShadowTex; }

// Commit Pixel-Perfect - debut
const sf::Image& RessourcesManager::getTopPipeImage() const { return topPipeImg; }
const sf::Image& RessourcesManager::getBottomPipeImage() const { return bottomPipeImg; }
// Commit Pixel-Perfect - fin
