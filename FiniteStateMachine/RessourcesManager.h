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

    const sf::Texture& getMidBgTexture() const;
    const sf::Texture& getFrontBgTexture() const;

    // Nouveaux getters pour les 3 variations de tuyaux
    const sf::Texture& getTopPipeTexture(int index) const;
    const sf::Texture& getBottomPipeTexture(int index) const;
    const std::string basePath = "Assets/";
    const sf::Font& getFont() const;
    //Background Texture 
    const sf::Texture& getMenuBgTexture() const;
    const sf::Texture& getSettingsBgTexture() const;
    const sf::Texture& getBgGameOverTexture() const;
    const sf::Texture& getShopTexture() const;
    const sf::Texture& getShopBgTexture() const;
    //Button Texture 
    const sf::Texture& getStartBtnTexture() const;
    const sf::Texture& getSettingsBtnTexture() const;
    const sf::Texture& getExitBtnTexture() const;
    const sf::Texture& getReturnBtnTexture() const;
    const sf::Texture& getBuyBtnTexture() const;
    const sf::Texture& getEquipBtnTexture() const;
    const sf::Texture& getEquippedBtnTexture() const;
    //player Texture 
    const sf::Texture& getGhostPlayerTexture() const;
    const sf::Texture& getBirdRedTexture() const;
    const sf::Texture& getBirdBlueTexture() const;
    const sf::Texture& getBirdGreenTexture() const;
    const sf::Texture& getBirdGoldTexture() const;
    const sf::Texture& getBirdShadowTexture() const;


    // Obstacles Texture 
    const sf::Image& getTopPipeImage(int index) const;
    const sf::Image& getBottomPipeImage(int index) const;

    const sf::Texture& getLaserTexture() const;
    const sf::Texture& getTitleTexture() const;



private:
    sf::Texture playerTex;
    sf::Texture bgTex;

    sf::Texture midBgTex;
    sf::Texture frontBgTex;

    // Tableaux pour stocker les 3 textures différentes
    sf::Texture topPipeTex[3];
    sf::Texture bottomPipeTex[3];
    sf::Font font;
    //Button Texture 
    sf::Texture startBtnTex;
    sf::Texture settingsBtnTex;
    sf::Texture exitBtnTex;
    sf::Texture returnBtnTex;
    sf::Texture equipBtnTex;
    sf::Texture equippedBtnTex;
    sf::Texture buyBtnTex;
    //Player Texture 
    sf::Texture ghostPlayerTex;
    sf::Texture birdRedTex;
    sf::Texture birdBlueTex;
    sf::Texture birdGreenTex;
    sf::Texture birdGoldTex;
    sf::Texture birdShadowTex;
    //Background Texture 
    sf::Texture settingsBgTex;
    sf::Texture shopTex;
    sf::Texture menuBgTex;
    sf::Texture bgGameOverTex;
    sf::Texture shopBgTex;
    //Obstacles Texture 
    sf::Image topPipeImg[3];
    sf::Image bottomPipeImg[3];
	
    sf::Texture laserTex;
    sf::Texture titleTex;

    bool loadTexture(sf::Texture& tex, const std::string& path);

};