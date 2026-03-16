#include "MainMenu.h"

MainMenu::MainMenu(const RessourcesManager& rm)
    : background(rm.getMenuBgTexture()),
    startBtn(rm.getStartBtnTexture()),
    settingsBtn(rm.getSettingsBtnTexture()),
    exitBtn(rm.getExitBtnTexture()),
    shopButton(rm.getShopTexture()),
    title(rm.getTitleTexture()),
    bestScoreText(rm.getFont(), "", 40),
    totalScoreText(rm.getFont(), "", 40),
    shopButtonBg(rm.getShopBgTexture())
{
    // Etirement du background
    float scaleX = 1920.f / rm.getMenuBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getMenuBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    title.setPosition({ 450.f, -50.f });
    title.setScale({ 0.30f, 0.30f });

    startBtn.setPosition({ 760.f, 430.f });
    startBtn.setScale({ 0.11f, 0.11f });

    settingsBtn.setPosition({ 795.f, 580.f });
    settingsBtn.setScale({ 0.09f, 0.09f });

    shopButtonBg.setPosition({ 790.f,680.f });
    shopButtonBg.setScale({ 0.09f,0.09f });
    shopButton.setPosition({ 825.f, 720.f });
    shopButton.setScale({ 0.07f, 0.07f });

    exitBtn.setPosition({ 790.f, 800.f });
    exitBtn.setScale({ 0.09f, 0.09f });

    startHitbox = sf::FloatRect({ 800.f, 525.f }, { 320.f, 90.f });
    settingsHitbox = sf::FloatRect({ 820.f, 645.f }, { 265.f, 80.f });
    shopHitbox = sf::FloatRect({ 885.f, 750.f }, { 158.f, 88.f });
    exitHitbox = sf::FloatRect({ 880.f, 850.f }, { 150.f, 90.f });

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

            if (startHitbox.contains(mPos)) return Action::Play;
            if (settingsHitbox.contains(mPos)) return Action::Options;
            if (shopHitbox.contains(mPos)) return Action::Shop;
            if (exitHitbox.contains(mPos)) return Action::Quit;
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
    window.draw(shopButtonBg);
    window.draw(shopButton);

    window.draw(bestScoreText);
    window.draw(totalScoreText);
    sf::RectangleShape debug;

    debug.setPosition({ startHitbox.position.x, startHitbox.position.y });
    debug.setSize({ startHitbox.size.x, startHitbox.size.y });
    debug.setFillColor(sf::Color(255, 0, 0, 80));
    window.draw(debug);

}