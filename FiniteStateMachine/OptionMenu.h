#pragma once
#include <SFML/Graphics.hpp>
#include "RessourcesManager.h"
#include "AudioManager.h" 

class OptionMenu {
public:
    enum class Action { None, Return };

    OptionMenu(const RessourcesManager& rm, AudioManager& am);
    Action handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

private:
    AudioManager& am; 

    sf::Sprite background;
    sf::Sprite returnBtn;

    sf::Text musicLabel;
    sf::RectangleShape musicBar;
    sf::CircleShape musicCursor;
    sf::Text musicVolText;
    sf::RectangleShape musicMuteBtn;
    sf::Text musicMuteText;

    sf::Text soundLabel;
    sf::RectangleShape soundBar;
    sf::CircleShape soundCursor;
    sf::Text soundVolText;
    sf::RectangleShape soundMuteBtn;
    sf::Text soundMuteText;

    bool isDraggingMusic = false;
    bool isDraggingSound = false;

    void updateUI();
};