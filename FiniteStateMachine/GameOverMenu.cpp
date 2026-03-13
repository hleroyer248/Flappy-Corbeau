#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(const RessourcesManager& rm)
    : background(rm.getMenuBgTexture()),
    retryBtn(rm.getStartBtnTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    score(rm.getFont(), "Score : 0", 80)
{
    // Etirement du background
    float scaleX = 1920.f / rm.getMenuBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getMenuBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    score.setFillColor(sf::Color::White);
    retryBtn.setPosition({ 760.f, 500.f });
    returnBtn.setPosition({ 760.f, 700.f });
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
    window.draw(retryBtn);
    window.draw(returnBtn);
}

void GameOverMenu::updateScoreText(int finalScore)
{
    score.setString("Score: " + std::to_string(finalScore));

    sf::FloatRect textRect = score.getLocalBounds();
    score.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
    score.setPosition({ 960.f, 300.f });
}