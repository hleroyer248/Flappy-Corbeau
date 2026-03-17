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

    if (!loadTexture(playerTex, basePath + "Player.png")) return false;
    if (!loadTexture(ghostPlayerTex, basePath + "Ghost-Player.png")) return false;
    if (!loadTexture(bgTex, basePath + "Background.png")) return false;
    if (!loadTexture(midBgTex, basePath + "MidBackground.png")) return false;
    if (!loadTexture(frontBgTex, basePath + "FrontBackground.png")) return false;
    if (!loadTexture(menuBgTex, basePath + "MenuBackground.png")) return false;
    if (!loadTexture(startBtnTex, basePath + "StartButton.png")) return false;
    if (!loadTexture(settingsBtnTex, basePath + "SettingsButton.png")) return false;
    if (!loadTexture(exitBtnTex, basePath + "ExitButton.png")) return false;
    if (!loadTexture(settingsBgTex, basePath + "SettingsBackground.png")) return false;
    if (!loadTexture(returnBtnTex, basePath + "ReturnButton.png")) return false;
    if (!loadTexture(shopTex, basePath + "ShopButton.png")) return false;
    if (!loadTexture(buyBtnTex, basePath + "BuyButton.jpg")) return false;
    if (!loadTexture(equipBtnTex, basePath + "EquipButton.png")) return false;
    if (!loadTexture(equippedBtnTex, basePath + "EquippedButton.png")) return false;
    if (!loadTexture(birdRedTex, basePath + "BirdRed.png")) return false;
    if (!loadTexture(birdBlueTex, basePath + "BirdBlue.png")) return false;
    if (!loadTexture(birdGreenTex, basePath + "BirdGreen.png")) return false;
    if (!loadTexture(birdGoldTex, basePath + "BirdGold.png")) return false;
    if (!loadTexture(bgGameOverTex, basePath + "BgGameOver.png")) return false;
    if (!loadTexture(birdShadowTex, basePath + "BirdShadow.png")) return false;
    if (!loadTexture(titleTex, basePath + "Title.png")) return false;
    if (!loadTexture(shopBgTex, basePath + "ShopButtonBG.png")) return false;

    // AUTO-CROP CHIFFRES
    sf::Image numbersImg;
    if (numbersImg.loadFromFile(basePath + "Chiffre.png") || numbersImg.loadFromFile(basePath + "Chiffre.jpg")) {
        numbersImg.createMaskFromColor(sf::Color::Black);
        if (numbersTex.loadFromImage(numbersImg)) {
            digitRects.clear();
            int w = numbersImg.getSize().x;
            int h = numbersImg.getSize().y;
            bool inDigit = false;
            int startX = 0;

            for (int x = 0; x < w; ++x) {
                bool hasPixel = false;
                for (int y = 0; y < h; ++y) {
                    if (numbersImg.getPixel({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) }).a > 0) {
                        hasPixel = true; break;
                    }
                }
                if (hasPixel && !inDigit) { inDigit = true; startX = x; }
                else if (!hasPixel && inDigit) {
                    inDigit = false;
                    digitRects.push_back(sf::IntRect({ startX, 0 }, { x - startX, h }));
                }
            }
            if (inDigit) digitRects.push_back(sf::IntRect({ startX, 0 }, { w - startX, h }));
            std::cout << "OK : Chiffre (Auto-Crop effectue)" << std::endl;
        }
    }

    // NOUVEAU : AUTO-CROP 2D LASER
    sf::Image laserImg;
    if (laserImg.loadFromFile(basePath + "Laser.png") || laserImg.loadFromFile(basePath + "Laser.jpg")) {
        laserImg.createMaskFromColor(sf::Color::Black);
        if (laserTex.loadFromImage(laserImg)) {
            laserRects.clear();
            int w = laserImg.getSize().x;
            int h = laserImg.getSize().y;

            bool inRow = false;
            int startY = 0;
            // Scan horizontal pour trouver les lignes
            for (int y = 0; y < h; ++y) {
                bool rowHasPixel = false;
                for (int x = 0; x < w; ++x) {
                    if (laserImg.getPixel({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) }).a > 0) {
                        rowHasPixel = true; break;
                    }
                }
                if (rowHasPixel && !inRow) {
                    inRow = true; startY = y;
                }
                else if (!rowHasPixel && inRow) {
                    inRow = false;
                    // Scan vertical au sein de cette ligne pour trouver les colonnes
                    bool inCol = false;
                    int startX = 0;
                    for (int x = 0; x < w; ++x) {
                        bool colHasPixel = false;
                        for (int ry = startY; ry < y; ++ry) {
                            if (laserImg.getPixel({ static_cast<unsigned int>(x), static_cast<unsigned int>(ry) }).a > 0) {
                                colHasPixel = true; break;
                            }
                        }
                        if (colHasPixel && !inCol) {
                            inCol = true; startX = x;
                        }
                        else if (!colHasPixel && inCol) {
                            inCol = false;
                            laserRects.push_back(sf::IntRect({ startX, startY }, { x - startX, y - startY }));
                        }
                    }
                    if (inCol) laserRects.push_back(sf::IntRect({ startX, startY }, { w - startX, y - startY }));
                }
            }
            // Sécurité si un seul laser a été trouvé
            if (laserRects.empty()) {
                laserRects.push_back(sf::IntRect({ 0, 0 }, { w, h }));
            }
            std::cout << "OK : Laser (Auto-Crop 2D effectue, " << laserRects.size() << " frames trouvees)" << std::endl;
        }
    }
    else {
        std::cerr << "ERREUR: Impossible de charger Laser.png\n";
        return false;
    }

    return true;
}

bool RessourcesManager::loadTexture(sf::Texture& tex, const std::string& path) {
    if (!tex.loadFromFile(path)) {
        std::cerr << "Erreur chargement : " << path << std::endl;
        return false;
    }
    std::cout << "OK : " << path << std::endl;
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

const sf::Image& RessourcesManager::getTopPipeImage(int index) const { return topPipeImg[index]; }
const sf::Image& RessourcesManager::getBottomPipeImage(int index) const { return bottomPipeImg[index]; }

const sf::Texture& RessourcesManager::getBgGameOverTexture() const { return bgGameOverTex; }
const sf::Texture& RessourcesManager::getLaserTexture() const { return laserTex; }
const sf::Texture& RessourcesManager::getTitleTexture() const { return titleTex; }
const sf::Texture& RessourcesManager::getShopBgTexture() const { return shopBgTex; }

const sf::Texture& RessourcesManager::getNumbersTexture() const { return numbersTex; }
const sf::IntRect& RessourcesManager::getDigitRect(int index) const {
    if (index >= 0 && index < digitRects.size()) return digitRects[index];
    static sf::IntRect defaultRect({ 0, 0 }, { 0, 0 });
    return defaultRect;
}

// METHODES LASER
const sf::IntRect& RessourcesManager::getLaserRect(int index) const {
    if (index >= 0 && index < laserRects.size()) return laserRects[index];
    static sf::IntRect defaultRect({ 0, 0 }, { 0, 0 });
    return defaultRect;
}

int RessourcesManager::getLaserFrameCount() const {
    return static_cast<int>(laserRects.size());
}