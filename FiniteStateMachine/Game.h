#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "RessourcesManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Obstacle.h"
#include "MainMenu.h"
#include "Boutique.h"
#include "OptionMenu.h"
#include "GameOverMenu.h"
#include "ParallaxLayer.h"
#include "Save.h" 
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

    void drawScore(sf::RenderWindow& window, int scoreVal, sf::Vector2f position, float scale, bool center);

    sf::RenderWindow window;
    RessourcesManager rm;

    AudioManager am;

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

    GameState state;
    int score;
    float pipeSpawnTimer;
    bool lastPipeWasMoving;
    bool debugMode = false;

    sf::Clock clock;
    std::mt19937 gen;
    std::uniform_real_distribution<float> gapDist;
    std::uniform_real_distribution<float> chanceDist;

    sf::Sprite capaIcon;
    sf::VertexArray cooldownArc;

    Save save;
};