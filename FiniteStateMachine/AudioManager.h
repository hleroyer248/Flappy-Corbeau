// Commit BugFix SFML 3 - debut
#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional> // Très important pour la SFML 3.0 !
#include <vector>
#include <random>

class AudioManager {
public:
    AudioManager();
    bool loadAll();

    void playMusic();
    void stopMusic();

    void playJumpSound();
    void playDeathSound();
    void playGhostSound();

    void playRandomMusic();
    void updateMusic(); 
    void updateCrow(float dt);
    void playLaserSound();
    void stopLaserSmooth();
    void updateLaser(float dt);



private:
    sf::Music bgMusic;

    sf::SoundBuffer jumpBuffer;
    std::optional<sf::Sound> jumpSound; // Doit absolument être un std::optional
    std::optional<sf::Sound> ghostSound;
    sf::SoundBuffer ghostBuffer;


    sf::SoundBuffer deathBuffer;
    std::optional<sf::Sound> deathSound; // Doit absolument être un std::optional

    std::vector<std::string> musicPaths;

    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
    std::uniform_real_distribution<float> chanceDist;
    float crowTimer = 0.f;
    std::uniform_real_distribution<float> timeDist;

    sf::SoundBuffer laserBuffer;
    std::optional<sf::Sound> laserSound;

    float laserVolume = 40.f;
    bool isFadingOut = false;
    float fadeSpeed = 40.f; // vitesse du fondu

};
// Commit BugFix SFML 3 - fin