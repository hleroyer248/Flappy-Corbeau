#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class MainMenu {
public:
    enum class Action { None, Play, Options, Quit };

    MainMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite settingsBtn;
    sf::Sprite exitBtn;
    sf::Text title;
};