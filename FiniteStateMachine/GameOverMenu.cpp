#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(const RessourcesManager& rm)
    : background(rm.getBgGameOverTexture()),
    retryBtn(rm.getStartBtnTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    score(rm.getFont(), " 0", 50),
    bestScoreText(rm.getFont(), "Best : 0", 40)
{

    // Positions pour centrer les boutons (� ajuster selon la taille de tes images)
    retryBtn.setPosition({ 450.f, 450.f });
    retryBtn.setScale({ 0.09f, 0.09f });
    returnBtn.setPosition({ 550.f, 700.f });
    returnBtn.setScale({ 0.09f, 0.09f });

   
    bestScoreText.setPosition({ 1200.f, 850.f });
    bestScoreText.setFillColor(sf::Color::White);

    background.setScale({ 2.0f, 2.0f });
    background.setPosition({ 100.f, 0.f });

}

GameOverMenu::Action GameOverMenu::handleEvent(const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));

            if (retryBtn.getGlobalBounds().contains(mPos)) return Action::Retry;
            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Quit;
        }
    }
    return Action::None;
}

void GameOverMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(score);
    window.draw(bestScoreText);
    window.draw(retryBtn);
    window.draw(returnBtn);
}

void GameOverMenu::updateScoreText(int finalScore)
{
    score.setString(std::to_string(finalScore));

    sf::FloatRect textRect = score.getLocalBounds();
    score.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
    score.setPosition({ 1250.f, 640.f });
}

void GameOverMenu::updateBestScore(int best)
{
    bestScoreText.setString(std::to_string(best));
}
