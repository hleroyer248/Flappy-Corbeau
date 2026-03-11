#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(const RessourcesManager& rm) 
    : background(rm.getMenuBgTexture()),
    retryBtn(rm.getStartBtnTexture()),
    exitBtn(rm.getExitBtnTexture()),
    score(rm.getFont(), "Score : 0", 50)
{
    score.setPosition({ 400.f, 300.f });
    score.setFillColor(sf::Color::White);

    // Positions pour centrer les boutons (à ajuster selon la taille de tes images)
    retryBtn.setPosition({ 300.f, 250.f });
    exitBtn.setPosition({ 300.f, 450.f });
}

GameOverMenu::Action GameOverMenu::handleEvent(const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));

            if (retryBtn.getGlobalBounds().contains(mPos)) return Action::Retry;
            if (exitBtn.getGlobalBounds().contains(mPos)) return Action::Quit;
        }
    }
    return Action::None;
}

void GameOverMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(score);
    window.draw(retryBtn);
    window.draw(exitBtn);
}

void GameOverMenu::updateScoreText(int finalScore)
{
    score.setString("Score: " + std::to_string(finalScore));

    sf::FloatRect textRect = score.getLocalBounds();
    score.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
    score.setPosition({ 400.f, 150.f });
}
