#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional> 
#include <vector>
#include <random>

class AudioManager {
public:
    AudioManager();
    bool loadAll();

    void playMusic();
    void stopMusic();

    void playJumpSound();
    void playGhostSound();

    void playRandomMusic();
    void updateMusic();
    void updateCrow(float dt);
    void playLaserSound();
    void updateLaser(float dt);


    void setMusicVolume(float vol);
    void setSoundVolume(float vol);
    void setMusicMute(bool mute);
    void setSoundMute(bool mute);

    float getMusicVolume() const;
    float getSoundVolume() const;
    bool getMusicMuted() const;
    bool getSoundMuted() const;

private:
    sf::Music bgMusic;

    sf::SoundBuffer jumpBuffer;
    std::optional<sf::Sound> jumpSound;
    std::optional<sf::Sound> ghostSound;
    sf::SoundBuffer ghostBuffer;

    std::vector<std::string> musicPaths;

    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
    std::uniform_real_distribution<float> chanceDist;
    float crowTimer = 0.f;
    std::uniform_real_distribution<float> timeDist;

    float masterMusicVolume;
    float masterSoundVolume;
    bool isMusicMuted;
    bool isSoundMuted;

    void updateSoundVolumes();
    struct LaserSound {
        sf::Sound sound;
        float lifetime = 0.f;
    };

    sf::SoundBuffer laserBuffer;
    std::vector<sf::Sound> laserShots;

};