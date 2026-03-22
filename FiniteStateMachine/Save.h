#pragma once
#include <string>
#include <vector>

class Save {
public:
    Save();
    void load();
    void saveToFile();
    void addScore(int newScore);
    int getBestScore() const;
    int getTotalScore() const;
    void spendCoins(int amount);

    std::vector<bool> ownedSkins;
    int equippedSkin;

    void buySkin(int index);
    void equipSkin(int index);

    bool isSkinOwned(int index) const;
    int getEquippedSkin() const;

    bool slowMotionOwned;
    bool slowMotionEquipped;

    bool isSlowMotionOwned() const;
    bool isSlowMotionEquipped() const;

    void buySlowMotion();
    void equipSlowMotion();

private:
    int bestScore;
    int totalScore;
    std::string filename;
};