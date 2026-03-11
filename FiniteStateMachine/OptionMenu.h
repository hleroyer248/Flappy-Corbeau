#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"

class OptionMenu {
public:
    enum class Action { None, Return };

    OptionMenu(const RessourcesManager& rm);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

    void update(const sf::RenderWindow& window); // Pour la logique du slider


private:
    sf::Sprite background;
    sf::Sprite returnBtn;
    sf::Text infoText;

    sf::RectangleShape sliderBar;
    sf::CircleShape sliderCursor;
    sf::Text VolumeText;

    bool isDraggingVolume = false;
    float volumeValue = 100.f; // De 0 à 100
};