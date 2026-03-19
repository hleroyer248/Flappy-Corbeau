#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class RessourcesManager {
public:

    RessourcesManager();
    bool loadAll();
    const sf::Texture& getPlayerTexture() const;
    const sf::Texture& getBackBgTexture() const;

    const sf::Texture& getMidBgTexture() const;
    const sf::Texture& getGroundBgTexture() const;

    const sf::Texture& getTopPipeTexture(int index) const;
    const sf::Texture& getBottomPipeTexture(int index) const;
    const std::string basePath = "Assets/";
    const sf::Font& getFont() const;

    const sf::Texture& getMenuBgTexture() const;
    const sf::Texture& getSettingsBgTexture() const;
    const sf::Texture& getBgGameOverTexture() const;
    const sf::Texture& getShopTexture() const;
    const sf::Texture& getShopBgTexture() const;
    const sf::Texture& getFrontTopTexture() const;
    const sf::Texture& getFrontBottomTexture() const;

    const sf::Texture& getStartBtnTexture() const;
    const sf::Texture& getSettingsBtnTexture() const;
    const sf::Texture& getExitBtnTexture() const;
    const sf::Texture& getReturnBtnTexture() const;
    const sf::Texture& getBuyBtnTexture() const;
    const sf::Texture& getEquipBtnTexture() const;
    const sf::Texture& getEquippedBtnTexture() const;

    const sf::Texture& getGhostPlayerTexture() const;
    const sf::Texture& getBirdRedTexture() const;
    const sf::Texture& getBirdBlueTexture() const;
    const sf::Texture& getBirdGreenTexture() const;
    const sf::Texture& getBirdGoldTexture() const;
    const sf::Texture& getBirdShadowTexture() const;

    const sf::Image& getTopPipeImage(int index) const;
    const sf::Image& getBottomPipeImage(int index) const;

    const sf::Texture& getLaserTexture() const;
    const sf::Texture& getTitleTexture() const;

    const sf::Texture& getTopHeadTexture(int index) const;
    const sf::Texture& getTopBodyTexture(int index) const;
    const sf::Texture& getBottomHeadTexture(int index) const;
    const sf::Texture& getBottomBodyTexture(int index) const;

    const sf::Texture& getNumbersTexture() const;
    const sf::IntRect& getDigitRect(int index) const;

    // NOUVEAU : Auto-Crop du Laser
    const sf::IntRect& getLaserRect(int index) const;
    int getLaserFrameCount() const;
    const sf::Texture& getCapaTexture() const;

private:
    sf::Texture playerTex;
    sf::Texture backBgTex;
    sf::Texture midBgTex;
    sf::Texture groundBgTex;

    sf::Texture topPipeTex[3];
    sf::Texture bottomPipeTex[3];
    sf::Font font;

    sf::Texture startBtnTex;
    sf::Texture settingsBtnTex;
    sf::Texture exitBtnTex;
    sf::Texture returnBtnTex;
    sf::Texture equipBtnTex;
    sf::Texture equippedBtnTex;
    sf::Texture buyBtnTex;

    sf::Texture ghostPlayerTex;
    sf::Texture birdRedTex;
    sf::Texture birdBlueTex;
    sf::Texture birdGreenTex;
    sf::Texture birdGoldTex;
    sf::Texture birdShadowTex;

    sf::Texture settingsBgTex;
    sf::Texture shopTex;
    sf::Texture menuBgTex;
    sf::Texture bgGameOverTex;
    sf::Texture shopBgTex;
    sf::Texture frontTopTex;
    sf::Texture frontBottomTex;

    sf::Image topPipeImg[3];
    sf::Image bottomPipeImg[3];
    sf::Texture laserTex;
    sf::Texture titleTex;

    sf::Texture topHeadTex[3];
    sf::Texture topBodyTex[3];
    sf::Texture bottomHeadTex[3];
    sf::Texture bottomBodyTex[3];
    sf::Image bottomHeadImg[3];
    sf::Image bottomBodyImg[3];

    sf::Texture numbersTex;
    std::vector<sf::IntRect> digitRects;

    // NOUVEAU
    std::vector<sf::IntRect> laserRects;

    sf::Texture capaTex;

    bool loadTexture(sf::Texture& tex, const std::string& path);
};