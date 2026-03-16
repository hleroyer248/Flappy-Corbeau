#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(const RessourcesManager& rm_in)
    : rm(rm_in),
    background(rm_in.getBgGameOverTexture()),
    retryBtn(rm_in.getStartBtnTexture()),
    returnBtn(rm_in.getReturnBtnTexture()),
    bestScoreLabel(rm_in.getFont(), "Best :", 40),
    finalScoreVal(0),
    bestScoreVal(0)
{
    retryBtn.setPosition({ 450.f, 450.f });
    retryBtn.setScale({ 0.09f, 0.09f });
    returnBtn.setPosition({ 550.f, 700.f });
    returnBtn.setScale({ 0.09f, 0.09f });

    // Emplacement d'origine du meilleur score
    bestScoreLabel.setPosition({ 1200.f, 850.f });
    bestScoreLabel.setFillColor(sf::Color::White);

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

void GameOverMenu::drawNumbers(sf::RenderWindow& window, int value, sf::Vector2f position, float scale, bool center) const {
    const sf::Texture& tex = rm.getNumbersTexture();
    if (tex.getSize().x == 0) return;

    std::string s = std::to_string(value);

    // Espacement dynamique légèrement augmenté pour les petites échelles
    float spacing = 20.f * scale;
    float totalWidth = 0;

    // Calcul de la largeur totale pour pouvoir centrer
    for (char c : s) {
        int d = c - '0';
        totalWidth += rm.getDigitRect(d).size.x * scale + spacing;
    }
    totalWidth -= spacing;

    float currentX = position.x;
    if (center) {
        currentX -= totalWidth / 2.f;
    }

    sf::Sprite spr(tex);
    spr.setScale({ scale, scale });

    for (char c : s) {
        int d = c - '0';
        sf::IntRect rect = rm.getDigitRect(d);

        spr.setTextureRect(rect);
        spr.setPosition({ currentX, position.y });
        window.draw(spr);

        // On avance la position X pour le prochain chiffre
        currentX += rect.size.x * scale + spacing;
    }
}

void GameOverMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(bestScoreLabel);
    window.draw(retryBtn);
    window.draw(returnBtn);

    // MODIFICATION DE L'ÉCHELLE : On passe de 1.2f à 0.4f (Le score de la partie)
    drawNumbers(window, finalScoreVal, { 1250.f, 500.f }, 0.15f, true);

    // MODIFICATION DE L'ÉCHELLE : On passe de 0.8f à 0.25f (Le meilleur score)
    drawNumbers(window, bestScoreVal, { 1350.f, 700.f }, 0.15f, false);
}

void GameOverMenu::updateScoreText(int finalScore)
{
    finalScoreVal = finalScore;
}

void GameOverMenu::updateBestScore(int best)
{
    bestScoreVal = best;
}