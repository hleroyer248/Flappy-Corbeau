#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class GameOverMenu
{
public:
    enum class Action { None, Retry, Quit };

    GameOverMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
    void updateScoreText(int finalScore);
    void updateBestScore(int best);

private:
    const RessourcesManager& rm;
    sf::Sprite background;
    sf::Sprite retryBtn;
    sf::Sprite returnBtn;

    sf::Text bestScoreLabel;

    int finalScoreVal;
    int bestScoreVal;

    void drawNumbers(sf::RenderWindow& window, int value, sf::Vector2f position, float scale, bool center) const;
};