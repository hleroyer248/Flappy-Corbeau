#include "Save.h"
#include <fstream>

Save::Save() : bestScore(0), totalScore(0), filename("save.txt") {
    load();
}

void Save::load() {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> bestScore >> totalScore;
        file.close();
    }
}

void Save::saveToFile() {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << bestScore << " " << totalScore;
        file.close();
    }
}

void Save::addScore(int newScore) {
    totalScore += newScore;
    if (newScore > bestScore) {
        bestScore = newScore;
    }
    saveToFile();
}

int Save::getBestScore() const {
    return bestScore;
}

int Save::getTotalScore() const {
    return totalScore;
}