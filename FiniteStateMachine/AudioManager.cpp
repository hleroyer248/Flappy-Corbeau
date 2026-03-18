#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    std::random_device rd;
    gen = std::mt19937(rd());
    chanceDist = std::uniform_real_distribution<float>(0.f, 100.f);
    timeDist = std::uniform_real_distribution<float>(2.f, 6.f); 
    crowTimer = timeDist(gen);
}

bool AudioManager::loadAll() {
    const std::string basePath = "Assets/";

    musicPaths = {
         basePath + "Music1.mp3",
         basePath + "Music2.mp3",
         basePath + "Music3.mp3",
         basePath + "Music4.mp3"
    };

    dist = std::uniform_int_distribution<int>(0, musicPaths.size() - 1);

    // 🔊 On lance une première musique
    playRandomMusic();

    std::cout << "[Audio] Playlist chargee !" << std::endl;

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
    deathSound->setVolume(0.f);


    if (!ghostBuffer.loadFromFile(basePath + "Ghost.wav")) {
        std::cerr << "ERREUR: Impossible de charger Ghost.mp3.\n";
        return false;
    }
    ghostSound.emplace(ghostBuffer);
    ghostSound->setVolume(150.f);
    ghostSound->setPitch(0.6f); 
    ghostSound->setRelativeToListener(false);
    ghostSound->setPosition({ 0.f, 0.f, -10.f }); 
    ghostSound->setMinDistance(5.f);

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

    if (chance < 80.f) {
        std::cout << "[Audio] Crow joue !" << std::endl;
        jumpSound->play();
    }
}

void AudioManager::playDeathSound() {
    if (deathSound) {
        std::cout << "[Audio] Bruit de mort (Dead) !" << std::endl;
        deathSound->play();
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
    bgMusic.setVolume(50.f);
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

        jumpSound->setPitch(0.8f + (chanceDist(gen) / 100.f) * 0.4f); // 🔥 variation cool
        jumpSound->play();

        crowTimer = timeDist(gen); // 🔁 nouveau temps random
    }
}