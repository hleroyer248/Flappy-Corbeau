#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "RessourcesManager.h"
#include "Player.h"
#include "Obstacle.h"
#include "MainMenu.h"
#include "OptionMenu.h"
#include "GameOverMenu.h"

// Ajout des nouveaux états
enum class GameState { MainMenu, OptionMenu, Ready, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetGame();

    sf::RenderWindow window;
    RessourcesManager rm;
    Player* player;
    std::vector<Obstacle> obstacles;

    std::optional<MainMenu> mainMenu;
    std::optional<OptionMenu> optionMenu;
    std::optional<GameOverMenu>gameOverMenu;

    std::optional<sf::Sprite> bg1;
    std::optional<sf::Sprite> bg2;
    float bgWidth;

    std::optional<sf::Text> menuTitle;
    std::optional<sf::Text> startButton;
    std::optional<sf::Text> scoreText;

    GameState state;
    int score;
    float pipeSpawnTimer;
    bool lastPipeWasMoving;

    sf::Clock clock;
    std::mt19937 gen;
    std::uniform_real_distribution<float> gapDist;
    std::uniform_real_distribution<float> chanceDist;
};