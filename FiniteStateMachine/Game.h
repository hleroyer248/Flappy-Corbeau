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
#include "Utils.h"
#include "GameEvent.h"
#include "SlowMotion.h"

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
    SlowMotion slowMotion;
    std::vector<Obstacle> obstacles;

    std::vector<ParallaxLayer> backLayers;
    std::vector<ParallaxLayer> frontLayers;

    std::optional<MainMenu> mainMenu;
    std::optional<OptionMenu> optionMenu;
    std::optional<GameOverMenu>gameOverMenu;
    std::optional<Boutique> shop;

    std::optional<sf::Text> menuTitle;
    std::optional<sf::Text> startButton;

    GameState state;
    int score;
    float pipeSpawnTimer;
    bool lastPipeWasMoving;
    bool debugMode = false;

    sf::Clock clock;
    sf::Clock rainbowClock;
    std::mt19937 gen;
    std::uniform_real_distribution<float> gapDist;
    std::uniform_real_distribution<float> chanceDist;

    sf::Sprite capaIcon;
    sf::Sprite slowIcon;

    std::optional<sf::Text> shiftTypeText;
    std::optional<sf::Text> slowTypeText;
    std::optional<sf::Text> slowCountdownText;
    sf::VertexArray slowCooldownArc;
    sf::VertexArray cooldownArc;

    Save save;

    bool laserWasActive = false;

    float slowCircleOffsetX = 5.f;
    float slowCircleOffsetY = 0.f;
    float slowCircleRadiusOffset = -15.f;
};
