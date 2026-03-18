// Commit BugFix SFML 3 - debut
#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional> // Très important pour la SFML 3.0 !

#include "OptionMenu.h"

class AudioManager {
public:
    AudioManager();
    bool loadAll();

    void playMusic();
    void stopMusic();

    void playJumpSound();
    void playDeathSound();

    void setVolume(float volume);
    void setVFXVolume(float volume);
private:

    sf::Music bgMusic;

    sf::SoundBuffer jumpBuffer;
    std::optional<sf::Sound> jumpSound; // Doit absolument être un std::optional

    sf::SoundBuffer deathBuffer;
    std::optional<sf::Sound> deathSound; // Doit absolument être un std::optional

    float vfxVolume = 100.f; // volume par défaut pour les VFX

};
