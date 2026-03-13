#include "MainMenu.h"

MainMenu::MainMenu(const RessourcesManager& rm)
    : background(rm.getMenuBgTexture()),
    startBtn(rm.getStartBtnTexture()),
    settingsBtn(rm.getSettingsBtnTexture()),
    exitBtn(rm.getExitBtnTexture()),
    title(rm.getFont(), "FLAPPY BIRD", 90),
    shopButton(rm.getShopTexture()),
    bestScoreText(rm.getFont(), "", 40),
    totalScoreText(rm.getFont(), "", 40)
{
    // Etirement du background
    float scaleX = 1920.f / rm.getMenuBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getMenuBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    title.setPosition({ 700.f, 200.f });
    title.setFillColor(sf::Color::White);

    startBtn.setPosition({ 760.f, 400.f });
    settingsBtn.setPosition({ 760.f, 550.f });
    shopButton.setPosition({ 760.f, 700.f });
    exitBtn.setPosition({ 760.f, 850.f });

    bestScoreText.setFillColor(sf::Color::Red);
    bestScoreText.setPosition({ 1500.f, 30.f });

    totalScoreText.setFillColor(sf::Color::Green);
    totalScoreText.setPosition({ 30.f, 30.f });
}

void MainMenu::updateScores(int bestScore, int totalScore) {
    bestScoreText.setString("Meilleur Score: " + std::to_string(bestScore));
    totalScoreText.setString("Score Total: " + std::to_string(totalScore));
}

MainMenu::Action MainMenu::handleEvent(const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));

            if (startBtn.getGlobalBounds().contains(mPos)) return Action::Play;
            if (settingsBtn.getGlobalBounds().contains(mPos)) return Action::Options;
            if (exitBtn.getGlobalBounds().contains(mPos)) return Action::Quit;
            if (shopButton.getGlobalBounds().contains(mPos)) return Action::Shop;
        }
    }
    return Action::None;
}

void MainMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(title);
    window.draw(startBtn);
    window.draw(settingsBtn);
    window.draw(exitBtn);
    window.draw(shopButton);

    window.draw(bestScoreText);
    window.draw(totalScoreText);
}