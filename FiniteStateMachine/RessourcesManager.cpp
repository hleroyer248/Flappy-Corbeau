#include "RessourcesManager.h"
#include <iostream>

RessourcesManager::RessourcesManager() {}

bool RessourcesManager::loadAll() {
    const std::string basePath = "Assets/";

    if (!font.openFromFile(basePath + "arial.ttf")) {
        std::cerr << "ERREUR: Impossible de charger arial.ttf depuis le dossier Assets.\n";
        return false;
    }

     for (int i = 0; i < 3; ++i) {
        std::string topName = "Hobstacle" + std::to_string(i + 1) + ".png";
        std::string botName = "Bobstacle" + std::to_string(i + 1) + ".png";

        if (!topPipeTex[i].loadFromFile(basePath + topName) ||
            !bottomPipeTex[i].loadFromFile(basePath + botName)) {
            std::cerr << "ERREUR: Impossible de charger " << topName << " ou " << botName << "\n";
            return false;
        }

        topPipeImg[i] = topPipeTex[i].copyToImage();
        bottomPipeImg[i] = bottomPipeTex[i].copyToImage();
    }

    if (!playerTex.loadFromFile(basePath + "Player.png") ||
        !ghostPlayerTex.loadFromFile(basePath + "Ghost-Player.png") ||
        !bgTex.loadFromFile(basePath + "Background.png") ||
        !midBgTex.loadFromFile(basePath + "MidBackground.png") ||
        !frontBgTex.loadFromFile(basePath + "FrontBackground.png") ||
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
        !bgGameOverTex.loadFromFile(basePath + "BgGameOver.png") ||
        !birdShadowTex.loadFromFile(basePath + "BirdShadow.png")) {
        std::cerr << "ERREUR: Impossible de charger un ou plusieurs assets png.\n";
        return false;
    }

    return true;
}

const sf::Texture& RessourcesManager::getPlayerTexture() const { return playerTex; }
const sf::Texture& RessourcesManager::getBgTexture() const { return bgTex; }
const sf::Texture& RessourcesManager::getMidBgTexture() const { return midBgTex; }
const sf::Texture& RessourcesManager::getFrontBgTexture() const { return frontBgTex; }

const sf::Texture& RessourcesManager::getTopPipeTexture(int index) const { return topPipeTex[index]; }
const sf::Texture& RessourcesManager::getBottomPipeTexture(int index) const { return bottomPipeTex[index]; }

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
const sf::Texture& RessourcesManager::getGhostPlayerTexture() const { return ghostPlayerTex; }
const sf::Texture& RessourcesManager::getBirdRedTexture() const { return birdRedTex; }
const sf::Texture& RessourcesManager::getBirdBlueTexture() const { return birdBlueTex; }
const sf::Texture& RessourcesManager::getBirdGreenTexture() const { return birdGreenTex; }
const sf::Texture& RessourcesManager::getBirdGoldTexture() const { return birdGoldTex; }
const sf::Texture& RessourcesManager::getBirdShadowTexture() const { return birdShadowTex; }

// Commit Pixel-Perfect - debut
const sf::Image& RessourcesManager::getTopPipeImage() const { return topPipeImg; }
const sf::Image& RessourcesManager::getBottomPipeImage() const { return bottomPipeImg; }
// Commit Pixel-Perfect - fin

const sf::Texture& RessourcesManager::getBgGameOverTexture() const{ return bgGameOverTex;}
