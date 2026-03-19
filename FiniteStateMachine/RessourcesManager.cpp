// ==========================================
// RessourcesManager.cpp
// ==========================================
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

        // 🔵 TOP (ON NE TOUCHE PAS)
        std::string topName = "Hobstacle" + std::to_string(i + 1) + ".png";

        if (!topPipeTex[i].loadFromFile(basePath + topName)) {
            std::cerr << "ERREUR: Impossible de charger " << topName << "\n";
            return false;
        }

        topPipeImg[i] = topPipeTex[i].copyToImage();

        // 🔴 BOTTOM HEAD (statue)
        std::string botHead = "Bobstacle" + std::to_string(i + 1) + ".png";

        if (!bottomHeadTex[i].loadFromFile(basePath + botHead)) {
            std::cerr << "ERREUR: Impossible de charger " << botHead << "\n";
            return false;
        }

        bottomHeadImg[i] = bottomHeadTex[i].copyToImage();

        // 🟡 BOTTOM BODY (partie étirable)
        std::string botBody = "SuitePylone" + std::to_string(i + 1) + ".png";

        if (!bottomBodyTex[i].loadFromFile(basePath + botBody)) {
            std::cerr << "ERREUR: Impossible de charger " << botBody << "\n";
            return false;
        }

        bottomBodyImg[i] = bottomBodyTex[i].copyToImage();
    }

    if (!loadTexture(playerTex, basePath + "Player.png")) return false;
    if (!loadTexture(ghostPlayerTex, basePath + "Ghost-Player.png")) return false;
    if (!loadTexture(backBgTex, basePath + "background_indoor.png")) return false;
    if (!loadTexture(midBgTex, basePath + "2nd_layer_B.png")) return false;
    if (!loadTexture(groundBgTex, basePath + "ground_layer.png")) return false;
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
    if (!loadTexture(capaTex, basePath + "Capa.png")) return false;
    if (!loadTexture(frontBottomTex, basePath + "FrontBottom.png")) return false;
    if (!loadTexture(frontTopTex, basePath + "FrontTop.png")) return false;

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

            // CORRECTION DU MERGE : La sécurité d'auto-crop des chiffres est de retour à sa bonne place !
            if (digitRects.size() < 10) {
                digitRects.clear();
                int defaultW = w / 10;
                for (int i = 0; i < 10; ++i) {
                    digitRects.push_back(sf::IntRect({ i * defaultW, 0 }, { defaultW, h }));
                }
            }

            std::cout << "OK : Chiffre (Auto-Crop effectue)" << std::endl;
        }
    }

    // AUTO-CROP HYBRIDE LASER
    sf::Image laserImg;
    if (laserImg.loadFromFile(basePath + "Laser.png") || laserImg.loadFromFile(basePath + "Laser.jpg")) {
        laserImg.createMaskFromColor(sf::Color::Black);
        if (laserTex.loadFromImage(laserImg)) {
            laserRects.clear();
            int w = laserImg.getSize().x;
            int h = laserImg.getSize().y;

            auto isVisible = [&](int px_x, int px_y) {
                sf::Color c = laserImg.getPixel({ static_cast<unsigned int>(px_x), static_cast<unsigned int>(px_y) });
                return (c.a > 20 && (c.r > 20 || c.g > 20 || c.b > 20));
                };

            int numCols = 2;
            int colWidth = w / numCols;

            for (int c = 0; c < numCols; ++c) {
                int startColX = c * colWidth;
                int endColX = startColX + colWidth;

                bool inLaser = false;
                int startY = 0;

                for (int y = 0; y < h; ++y) {
                    bool rowHasPixel = false;

                    for (int x = startColX; x < endColX; ++x) {
                        if (isVisible(x, y)) {
                            rowHasPixel = true;
                            break;
                        }
                    }

                    if (rowHasPixel && !inLaser) {
                        inLaser = true;
                        startY = y;
                    }
                    else if (!rowHasPixel && inLaser) {
                        inLaser = false;
                        int endY = y;

                        int trueMinX = endColX;
                        int trueMaxX = startColX;

                        for (int ry = startY; ry < endY; ++ry) {
                            for (int rx = startColX; rx < endColX; ++rx) {
                                if (isVisible(rx, ry)) {
                                    if (rx < trueMinX) trueMinX = rx;
                                    if (rx > trueMaxX) trueMaxX = rx;
                                }
                            }
                        }

                        int fW = trueMaxX - trueMinX + 1;
                        int fH = endY - startY;

                        if (fW > 50 && fH > 5) {
                            int fX = std::max(startColX, trueMinX - 2);
                            int fY = std::max(0, startY - 2);
                            fW = std::min(endColX - fX, fW + 4);
                            fH = std::min(h - fY, fH + 4);

                            laserRects.push_back(sf::IntRect({ fX, fY }, { fW, fH }));
                        }
                    }
                }

                if (inLaser) {
                    int endY = h;
                    int trueMinX = endColX;
                    int trueMaxX = startColX;
                    for (int ry = startY; ry < endY; ++ry) {
                        for (int rx = startColX; rx < endColX; ++rx) {
                            if (isVisible(rx, ry)) {
                                if (rx < trueMinX) trueMinX = rx;
                                if (rx > trueMaxX) trueMaxX = rx;
                            }
                        }
                    }

                    int fW = trueMaxX - trueMinX + 1;
                    int fH = endY - startY;

                    if (fW > 50 && fH > 5) {
                        int fX = std::max(startColX, trueMinX - 2);
                        int fY = std::max(0, startY - 2);
                        fW = std::min(endColX - fX, fW + 4);
                        fH = std::min(h - fY, fH + 4);

                        laserRects.push_back(sf::IntRect({ fX, fY }, { fW, fH }));
                    }
                }
            }

            // CORRECTION DU MERGE : On referme correctement le if et on retire le bloc des chiffres qui était coincé ici
            if (laserRects.empty()) {
                laserRects.push_back(sf::IntRect({ 0, 0 }, { w, h }));
            }
            std::cout << "OK : Laser (Decoupe Dynamique Parfaite, " << laserRects.size() << " frames trouvees)" << std::endl;
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
const sf::Texture& RessourcesManager::getBackBgTexture() const { return backBgTex; }
const sf::Texture& RessourcesManager::getMidBgTexture() const { return midBgTex; }
const sf::Texture& RessourcesManager::getGroundBgTexture() const { return groundBgTex; }
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
const sf::Texture& RessourcesManager::getCapaTexture() const { return capaTex; }

const sf::Texture& RessourcesManager::getTopHeadTexture(int index) const { return topHeadTex[index]; }
const sf::Texture& RessourcesManager::getTopBodyTexture(int index) const { return topBodyTex[index]; }
const sf::Texture& RessourcesManager::getBottomHeadTexture(int index) const { return bottomHeadTex[index]; }
const sf::Texture& RessourcesManager::getBottomBodyTexture(int index) const { return bottomBodyTex[index]; }

const sf::Texture& RessourcesManager::getNumbersTexture() const { return numbersTex; }
const sf::IntRect& RessourcesManager::getDigitRect(int index) const {
    if (index >= 0 && index < digitRects.size()) return digitRects[index];
    static sf::IntRect defaultRect({ 0, 0 }, { 0, 0 });
    return defaultRect;
}

const sf::IntRect& RessourcesManager::getLaserRect(int index) const {
    if (index >= 0 && index < laserRects.size()) return laserRects[index];
    static sf::IntRect defaultRect({ 0, 0 }, { 0, 0 });
    return defaultRect;
}

int RessourcesManager::getLaserFrameCount() const {
    return static_cast<int>(laserRects.size());
}

const sf::Texture& RessourcesManager::getFrontTopTexture() const { return frontTopTex; }
const sf::Texture& RessourcesManager::getFrontBottomTexture() const { return frontBottomTex; }
