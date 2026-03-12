#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {}

bool AudioManager::loadAll() {
    const std::string basePath = "Assets/";

    if (!bgMusic.openFromFile(basePath + "Music.mp3")) {
        std::cerr << "ERREUR: Impossible de charger Music.mp3.\n";
        return false;
    }
    bgMusic.setLooping(true);
    bgMusic.setVolume(20.f); // On force le volume à fond

    if (!jumpBuffer.loadFromFile(basePath + "Crow.mp3")) {
        std::cerr << "ERREUR: Impossible de charger Crow.mp3.\n";
        return false;
    }
    jumpSound.emplace(jumpBuffer);
    jumpSound->setVolume(30.f);

    if (!deathBuffer.loadFromFile(basePath + "Dead.mp3")) {
        std::cerr << "ERREUR: Impossible de charger Dead.mp3.\n";
        return false;
    }
    deathSound.emplace(deathBuffer);
    deathSound->setVolume(50.f);

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
    if (jumpSound) {
        std::cout << "[Audio] Bruit de saut (Crow) !" << std::endl;
        jumpSound->play();
    }
}

void AudioManager::playDeathSound() {
    if (deathSound) {
        std::cout << "[Audio] Bruit de mort (Dead) !" << std::endl;
        deathSound->play();
    }
}