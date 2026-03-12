// Commit BugFix SFML 3 - debut
#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional> // Très important pour la SFML 3.0 !

class AudioManager {
public:
    AudioManager();
    bool loadAll();

    void playMusic();
    void stopMusic();

    void playJumpSound();
    void playDeathSound();

private:
    sf::Music bgMusic;

    sf::SoundBuffer jumpBuffer;
    std::optional<sf::Sound> jumpSound; // Doit absolument être un std::optional

    sf::SoundBuffer deathBuffer;
    std::optional<sf::Sound> deathSound; // Doit absolument être un std::optional
};
// Commit BugFix SFML 3 - fin