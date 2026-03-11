#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class MainMenu {
public:
    enum class Action { None, Play, Shop, Options, Quit };

    MainMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
void updateScores(int bestScore, int totalScore);

private:
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite settingsBtn;
    sf::Sprite exitBtn;
    sf::Sprite shopButton;
    sf::Text title;
sf::Text bestScoreText;  
    sf::Text totalScoreText; 
};
