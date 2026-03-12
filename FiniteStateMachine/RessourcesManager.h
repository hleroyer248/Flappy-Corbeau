#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class RessourcesManager {
public:

    RessourcesManager();
    bool loadAll();
    const sf::Texture& getPlayerTexture() const;
    const sf::Texture& getBgTexture() const;
    const sf::Texture& getTopPipeTexture() const;
    const sf::Texture& getBottomPipeTexture() const;
    const sf::Font& getFont() const;

    const sf::Texture& getMenuBgTexture() const;
    const sf::Texture& getStartBtnTexture() const;
    const sf::Texture& getSettingsBtnTexture() const;
    const sf::Texture& getExitBtnTexture() const;
    const sf::Texture& getSettingsBgTexture() const;
    const sf::Texture& getReturnBtnTexture() const;
    const sf::Texture& getShopTexture() const;
    const sf::Texture& getBuyBtnTexture() const;
    const sf::Texture& getEquipBtnTexture() const;
    const sf::Texture& getEquippedBtnTexture() const;
    const sf::Texture& getGhostPlayerTexture() const; // Commit Ghost
    const sf::Texture& getBirdRedTexture() const;
    const sf::Texture& getBirdBlueTexture() const;
    const sf::Texture& getBirdGreenTexture() const;
    const sf::Texture& getBirdGoldTexture() const;
    const sf::Texture& getBirdShadowTexture() const;
    const sf::Texture& getDefaultBirdTexture() const;

private:
    sf::Texture playerTex;
    sf::Texture bgTex;
    sf::Texture topPipeTex;
    sf::Texture bottomPipeTex;
    sf::Font font;

    sf::Texture menuBgTex;
    sf::Texture startBtnTex;
    sf::Texture settingsBtnTex;
    sf::Texture exitBtnTex;
    sf::Texture settingsBgTex;
    sf::Texture returnBtnTex;
    sf::Texture shopTex;
    sf::Texture equipBtnTex;
    sf::Texture equippedBtnTex;
    sf::Texture buyBtnTex;
    sf::Texture ghostPlayerTex; // Commit Ghost
    sf::Texture birdRedTex;
    sf::Texture birdBlueTex;
    sf::Texture birdGreenTex;
    sf::Texture birdGoldTex;
    sf::Texture birdShadowTex;
    sf::Texture defaultBirdTex;
};
