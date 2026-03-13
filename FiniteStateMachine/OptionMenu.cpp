#include "OptionMenu.h"
#include <SFML/Graphics.hpp>

OptionMenu::OptionMenu(const RessourcesManager& rm)
    : background(rm.getSettingsBgTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    infoText(rm.getFont(), "OPTIONS", 80), VolumeText(rm.getFont(), "50%", 40) {

    // Etirement du background
    float scaleX = 1920.f / rm.getSettingsBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getSettingsBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    infoText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = infoText.getLocalBounds();
    infoText.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
    infoText.setPosition({ 960.f, 200.f });

    VolumeText.setFillColor(sf::Color::Green);
    VolumeText.setPosition({ 1200.f, 485.f });

    sliderBar.setSize({ 400.f, 20.f });
    sliderBar.setFillColor(sf::Color(100, 100, 100));
    sliderBar.setPosition({ 760.f, 500.f });

    sliderCursor.setRadius(25.f);
    sliderCursor.setOrigin({ 25.f, 25.f });
    sliderCursor.setFillColor(sf::Color::White);
    sliderCursor.setPosition({ 960.f, 510.f });

    returnBtn.setPosition({ 760.f, 800.f });
}

OptionMenu::Action OptionMenu::handleEvent(const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Return;
        }
    }

    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));
            if (sliderCursor.getGlobalBounds().contains(mPos)) isDraggingVolume = true;
        }
    }

    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (isDraggingVolume) {
            float mouseX = static_cast<float>(mouseMove->position.x);
            float minX = 760.f;
            float maxX = 1160.f;

            if (mouseX < minX) mouseX = minX;
            if (mouseX > maxX) mouseX = maxX;

            sliderCursor.setPosition({ mouseX, 510.f });

            float ratio = (mouseX - minX) / 400.f;
            volumeValue = ratio * 100.f;

            VolumeText.setString(std::to_string(static_cast<int>(volumeValue)) + "%");
        }
    }

    if (event.is<sf::Event::MouseButtonReleased>()) {
        isDraggingVolume = false;
        if (const auto* mouseUp = event.getIf<sf::Event::MouseButtonReleased>()) {
            sf::Vector2f mPos(static_cast<float>(mouseUp->position.x), static_cast<float>(mouseUp->position.y));
            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Return;
        }
    }

    return Action::None;
}

void OptionMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(infoText);
    window.draw(sliderBar);
    window.draw(sliderCursor);
    window.draw(VolumeText);
    window.draw(returnBtn);
}