#pragma once
#include <string>

class Save {
public:
    Save();
    void load();
    void saveToFile();
    void addScore(int newScore);
    int getBestScore() const;
    int getTotalScore() const;

private:
    int bestScore;
    int totalScore;
    std::string filename;
};