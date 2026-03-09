#include "OptionMenu.h"

OptionMenu::OptionMenu(const RessourcesManager& rm)
    : background(rm.getSettingsBgTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    infoText(rm.getFont(), "Sprint progress", 40) {

    infoText.setPosition({ 250.f, 250.f });
    infoText.setFillColor(sf::Color::White);

    // Bouton retour placé en bas à droite
    returnBtn.setPosition({ 600.f, 500.f });
}

OptionMenu::Action OptionMenu::handleEvent(const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));

            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Return;
        }
    }
    return Action::None;
}

void OptionMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(infoText);
    window.draw(returnBtn);
}