#include "Save.h"
#include <fstream>

Save::Save() : bestScore(0), totalScore(0), filename("save.txt"), equippedSkin(-1) {
    ownedSkins.resize(7, false);
    load();
}

void Save::load() {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> bestScore >> totalScore;
        for (int i = 0; i < ownedSkins.size(); i++)
        {
            int value;
            file >> value;
            ownedSkins[i] = (value == 1);
        }

        file >> equippedSkin;
        file.close();
    }
}

void Save::saveToFile() {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << bestScore << " " << totalScore << " ";
        for (bool owned : ownedSkins)
            file << owned << " ";

        file << equippedSkin;
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

void Save::spendCoins(int amount)
{
    totalScore -= amount;
    saveToFile();
}

void Save::buySkin(int index)
{
    ownedSkins[index] = true;
    saveToFile();
}


void Save::equipSkin(int index)
{
    equippedSkin = index;
    saveToFile();
}


bool Save::isSkinOwned(int index) const
{
    return ownedSkins[index];
}

int Save::getEquippedSkin() const
{
    return equippedSkin;
}