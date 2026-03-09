#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class OptionMenu {
public:
    enum class Action { None, Return };

    OptionMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite background;
    sf::Sprite returnBtn;
    sf::Text infoText;
};