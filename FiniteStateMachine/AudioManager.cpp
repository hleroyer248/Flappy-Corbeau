#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    std::random_device rd;
    gen = std::mt19937(rd());
    chanceDist = std::uniform_real_distribution<float>(0.f, 100.f);
    timeDist = std::uniform_real_distribution<float>(2.f, 6.f);
    crowTimer = timeDist(gen);

    masterMusicVolume = 50.f;
    masterSoundVolume = 100.f;
    isMusicMuted = false;
    isSoundMuted = false;
}

bool AudioManager::loadAll() {
    const std::string basePath = "Assets/";

    musicPaths = {
         basePath + "Music1.mp3",
         basePath + "Music2.mp3",
         basePath + "Music3.mp3",
         basePath + "Music4.mp3"
    };

    dist = std::uniform_int_distribution<int>(0, static_cast<int>(musicPaths.size()) - 1);

    playRandomMusic();

    std::cout << "[Audio] Playlist chargee !" << std::endl;

    if (!jumpBuffer.loadFromFile(basePath + "Crow.mp3")) {
        std::cerr << "ERREUR: Impossible de charger Crow.mp3.\n";
        return false;
    }
    jumpSound.emplace(jumpBuffer);

    if (!laserBuffer.loadFromFile(basePath + "Laser.mp3")) {
        std::cerr << "ERREUR: Impossible de charger Laser.mp3.\n";
        return false;
    }

    if (!ghostBuffer.loadFromFile(basePath + "Ghost.wav")) {
        std::cerr << "ERREUR: Impossible de charger Ghost.wav.\n";
        return false;
    }
    ghostSound.emplace(ghostBuffer);
    ghostSound->setPitch(0.6f);
    ghostSound->setRelativeToListener(false);
    ghostSound->setPosition({ 0.f, 0.f, -10.f });
    ghostSound->setMinDistance(5.f);

    updateSoundVolumes();

    std::cout << "[Audio] Fichiers charges avec succes !" << std::endl;
    return true;
}

void AudioManager::playMusic() {
    std::cout << "[Audio] Lancement de la musique !" << std::endl;
    bgMusic.play();
}

void AudioManager::stopMusic() {
    std::cout << "[Audio] Arret de la musique." << std::endl;
    bgMusic.stop();
}

void AudioManager::playJumpSound() {
    if (!jumpSound) return;

    float chance = chanceDist(gen);
    if (chance < 5.f) {
        std::cout << "[Audio] Crow joue !" << std::endl;
        jumpSound->play();
    }
}

void AudioManager::playGhostSound() {
    if (ghostSound) {
        ghostSound->setPlayingOffset(sf::seconds(1.4f));
        ghostSound->play();
    }
}

void AudioManager::playRandomMusic() {
    int index = dist(gen);

    if (!bgMusic.openFromFile(musicPaths[index])) {
        std::cerr << "ERREUR: Impossible de charger musique.\n";
        return;
    }

    bgMusic.setLooping(false);
    bgMusic.setVolume(isMusicMuted ? 0.f : masterMusicVolume);
    bgMusic.play();

    std::cout << "[Audio] Lecture musique : " << musicPaths[index] << std::endl;
}

void AudioManager::updateMusic() {
    if (bgMusic.getStatus() == sf::SoundSource::Status::Stopped) {
        playRandomMusic();
    }
}

void AudioManager::updateCrow(float dt) {
    if (!jumpSound) return;

    crowTimer -= dt;
    if (crowTimer <= 0.f) {
        std::cout << "[Audio] Crow aleatoire !" << std::endl;
        jumpSound->setPitch(0.8f + (chanceDist(gen) / 100.f) * 0.4f);
        jumpSound->play();
        crowTimer = timeDist(gen);
    }
}

void AudioManager::setMusicVolume(float vol) {
    masterMusicVolume = vol;
    if (!isMusicMuted) bgMusic.setVolume(masterMusicVolume);
}

void AudioManager::setSoundVolume(float vol) {
    masterSoundVolume = vol;
    updateSoundVolumes();
}

void AudioManager::setMusicMute(bool mute) {
    isMusicMuted = mute;
    bgMusic.setVolume(isMusicMuted ? 0.f : masterMusicVolume);
}

void AudioManager::setSoundMute(bool mute) {
    isSoundMuted = mute;
    updateSoundVolumes();
}

float AudioManager::getMusicVolume() const { return masterMusicVolume; }
float AudioManager::getSoundVolume() const { return masterSoundVolume; }
bool AudioManager::getMusicMuted() const { return isMusicMuted; }
bool AudioManager::getSoundMuted() const { return isSoundMuted; }

void AudioManager::updateSoundVolumes() {
    float mult = isSoundMuted ? 0.f : (masterSoundVolume / 100.f);

    if (jumpSound) jumpSound->setVolume(30.f * mult);
    if (ghostSound) ghostSound->setVolume(150.f * mult);

    for (auto& sound : laserShots) {
        sound.setVolume(50.f * mult);
    }
}

void AudioManager::playLaserSound() {
    laserShots.emplace_back(laserBuffer);

    sf::Sound& sound = laserShots.back();

    float mult = isSoundMuted ? 0.f : (masterSoundVolume / 100.f);
    sound.setVolume(50.f * mult);

    sound.setPitch(0.95f + (chanceDist(gen) / 100.f) * 0.1f);
    sound.setPlayingOffset(sf::seconds(1.2f));

    sound.play();
}


void AudioManager::updateLaser(float dt) {
    laserShots.erase(
        std::remove_if(laserShots.begin(), laserShots.end(),
            [](const sf::Sound& s) {
                return s.getStatus() == sf::SoundSource::Status::Stopped;
            }),
        laserShots.end()
    );
}