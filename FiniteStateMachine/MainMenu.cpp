#include "MainMenu.h"

MainMenu::MainMenu(const RessourcesManager& rm)
    : background(rm.getMenuBgTexture()),
    startBtn(rm.getStartBtnTexture()),
    settingsBtn(rm.getSettingsBtnTexture()),
    exitBtn(rm.getExitBtnTexture()),
    title(rm.getFont(), "FLAPPY BIRD", 50),
    shopButton(rm.getShopTexture()), // commit sauvegarde 
    bestScoreText(rm.getFont(), "", 24),    // commit sauvegarde
    totalScoreText(rm.getFont(), "", 24)    // commit sauvegarde
{

    title.setPosition({ 230.f, 100.f });
    title.setFillColor(sf::Color::White);

    startBtn.setPosition({ 300.f, 270.f }); //c'était a 250.f si vous voulez le remettre comme avant
    settingsBtn.setPosition({ 305.f, 330.f });
    exitBtn.setPosition({ 300.f, 450.f });
    shopButton.setPosition({ 355.f, 395.f });
    shopButton.setScale({ 1.f,1.f });

    // commit sauvegarde 
    bestScoreText.setFillColor(sf::Color::Red);
    bestScoreText.setPosition({ 300.f, 20.f });

    totalScoreText.setFillColor(sf::Color::Green);
    totalScoreText.setPosition({ 10.f, 10.f });
}

// commit sauvegarde 
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

    // commit sauvegarde 
    window.draw(bestScoreText);
    window.draw(totalScoreText);
}
