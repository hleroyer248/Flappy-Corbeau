#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class MainMenu {
public:
    enum class Action { None, Play, Shop, Options, Quit };

    MainMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
    void updateScores(int bestScore, int totalScore); // commit sauvegarde

private:
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite settingsBtn;
    sf::Sprite exitBtn;
    sf::Sprite shopButton;
    sf::Sprite title;
    sf::Text bestScoreText;  // commit sauvegarde
    sf::Text totalScoreText; // commit sauvegarde
    //Hitbox zone cliquable btn 
    sf::FloatRect startHitbox;
    sf::FloatRect settingsHitbox;
    sf::FloatRect shopHitbox;
    sf::FloatRect exitHitbox;
    sf::Sprite shopButtonBg;
};
