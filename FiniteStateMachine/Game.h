#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "RessourcesManager.h"
// Commit Add Sound - debut
#include "AudioManager.h"
// Commit Add Sound - fin
#include "Player.h"
#include "Obstacle.h"
#include "MainMenu.h"
#include "Boutique.h"
#include "OptionMenu.h"
#include "GameOverMenu.h"
#include "ParallaxLayer.h"
#include "Save.h" // commit sauvegarde

#include "GameEvent.h"

enum class GameState { MainMenu, OptionMenu, Shop, Ready, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:

    std::optional<GameEvent> gameEvent;

    void processEvents();
    void update(float dt);
    void render();
    void resetGame();

    sf::RenderWindow window;
    RessourcesManager rm;

    // Commit Add Sound - debut
    AudioManager am;
    // Commit Add Sound - fin

    Player* player;
    std::vector<Obstacle> obstacles;

    std::vector<ParallaxLayer> backLayers;
    std::vector<ParallaxLayer> frontLayers;

    std::optional<MainMenu> mainMenu;
    std::optional<OptionMenu> optionMenu;
    std::optional<GameOverMenu>gameOverMenu;
    std::optional<Boutique> shop;

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

    Save save; // commit sauvegarde
};  