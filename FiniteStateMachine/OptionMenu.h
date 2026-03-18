#pragma once
#include <SFML/Graphics.hpp>

#include "RessourcesManager.h"
#include "AudioManager.h"

class OptionMenu {

public:

    enum class Action { None, Return };

    OptionMenu(const RessourcesManager& rm);

    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
    void update(const sf::RenderWindow& window);
    
    
    float getVolume() const;

private:

    sf::Sprite background;
    sf::Sprite returnBtn;

    sf::Text infoText;
    sf::Text VolumeText;

    sf::RectangleShape sliderBar;
    sf::CircleShape sliderCursor;

    bool isDraggingVolume = false;
    float volumeValue = 100.f; // De 0 à 100

    sf::RectangleShape vfxSliderBar;
    sf::CircleShape vfxSliderCursor;
    sf::Text VFXVolumeText;

    bool isDraggingVFX = false;
    float vfxVolumeValue = 100.f; // volume initial VFX
};