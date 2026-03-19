#include "OptionMenu.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

OptionMenu::OptionMenu(const RessourcesManager& rm, AudioManager& am)
    : am(am),
    background(rm.getMenuBgTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    musicLabel(rm.getFont(), "Music", 50),
    soundLabel(rm.getFont(), "Sound", 50),
    musicVolText(rm.getFont(), "", 40),
    soundVolText(rm.getFont(), "", 40),
    musicMuteText(rm.getFont(), "", 30),
    soundMuteText(rm.getFont(), "", 30)
{
    float scaleX = 1920.f / rm.getMenuBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getMenuBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    musicLabel.setFillColor(sf::Color::White);
    musicLabel.setPosition({ 500.f, 300.f });

    musicBar.setSize({ 400.f, 20.f });
    musicBar.setFillColor(sf::Color(100, 100, 100));
    musicBar.setPosition({ 760.f, 320.f });

    musicCursor.setRadius(25.f);
    musicCursor.setOrigin({ 25.f, 25.f });
    musicCursor.setFillColor(sf::Color::White);

    musicVolText.setFillColor(sf::Color::Green);
    musicVolText.setPosition({ 1200.f, 305.f });

    musicMuteBtn.setSize({ 150.f, 60.f });
    musicMuteBtn.setPosition({ 1350.f, 300.f });

    musicMuteText.setFillColor(sf::Color::White);

    soundLabel.setFillColor(sf::Color::White);
    soundLabel.setPosition({ 500.f, 500.f });

    soundBar.setSize({ 400.f, 20.f });
    soundBar.setFillColor(sf::Color(100, 100, 100));
    soundBar.setPosition({ 760.f, 520.f });

    soundCursor.setRadius(25.f);
    soundCursor.setOrigin({ 25.f, 25.f });
    soundCursor.setFillColor(sf::Color::White);

    soundVolText.setFillColor(sf::Color::Green);
    soundVolText.setPosition({ 1200.f, 505.f });

    soundMuteBtn.setSize({ 150.f, 60.f });
    soundMuteBtn.setPosition({ 1350.f, 500.f });

    soundMuteText.setFillColor(sf::Color::White);

    returnBtn.setPosition({ 700.f, 800.f });
    returnBtn.setScale({ 0.09f, 0.09f });

    updateUI();
}

void OptionMenu::updateUI() {
    float mVol = am.getMusicVolume();
    musicCursor.setPosition({ 760.f + (mVol / 100.f) * 400.f, 330.f });
    musicVolText.setString(std::to_string(static_cast<int>(mVol)) + "%");

    if (am.getMusicMuted()) {
        musicMuteBtn.setFillColor(sf::Color(200, 50, 50));
        musicMuteText.setString("MUTED");
        musicMuteText.setPosition({ 1370.f, 310.f });
    }
    else {
        musicMuteBtn.setFillColor(sf::Color(50, 200, 50));
        musicMuteText.setString("ON");
        musicMuteText.setPosition({ 1400.f, 310.f });
    }

    float sVol = am.getSoundVolume();
    soundCursor.setPosition({ 760.f + (sVol / 100.f) * 400.f, 530.f });
    soundVolText.setString(std::to_string(static_cast<int>(sVol)) + "%");

    if (am.getSoundMuted()) {
        soundMuteBtn.setFillColor(sf::Color(200, 50, 50));
        soundMuteText.setString("MUTED");
        soundMuteText.setPosition({ 1370.f, 510.f });
    }
    else {
        soundMuteBtn.setFillColor(sf::Color(50, 200, 50));
        soundMuteText.setString("ON");
        soundMuteText.setPosition({ 1400.f, 510.f });
    }
}

OptionMenu::Action OptionMenu::handleEvent(const sf::Event& event) {
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));

            sf::FloatRect musicHitbox({ 740.f, 300.f }, { 440.f, 60.f });
            sf::FloatRect soundHitbox({ 740.f, 500.f }, { 440.f, 60.f });

            if (musicHitbox.contains(mPos)) {
                isDraggingMusic = true;
                float ratio = std::max(0.f, std::min((mPos.x - 760.f) / 400.f, 1.f));
                am.setMusicVolume(ratio * 100.f);
                updateUI();
            }
            else if (soundHitbox.contains(mPos)) {
                isDraggingSound = true;
                float ratio = std::max(0.f, std::min((mPos.x - 760.f) / 400.f, 1.f));
                am.setSoundVolume(ratio * 100.f);
                updateUI();
            }
            else if (musicMuteBtn.getGlobalBounds().contains(mPos)) {
                am.setMusicMute(!am.getMusicMuted());
                updateUI();
            }
            else if (soundMuteBtn.getGlobalBounds().contains(mPos)) {
                am.setSoundMute(!am.getSoundMuted());
                updateUI();
            }
        }
    }

    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        float mouseX = std::max(760.f, std::min(static_cast<float>(mouseMove->position.x), 1160.f));

        if (isDraggingMusic) {
            am.setMusicVolume(((mouseX - 760.f) / 400.f) * 100.f);
            updateUI();
        }
        else if (isDraggingSound) {
            am.setSoundVolume(((mouseX - 760.f) / 400.f) * 100.f);
            updateUI();
        }
    }

    if (const auto* mouseUp = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseUp->button == sf::Mouse::Button::Left) {
            isDraggingMusic = false;
            isDraggingSound = false;

            sf::Vector2f mPos(static_cast<float>(mouseUp->position.x), static_cast<float>(mouseUp->position.y));
            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Return;
        }
    }

    return Action::None;
}

void OptionMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);

    window.draw(musicLabel);
    window.draw(musicBar);
    window.draw(musicCursor);
    window.draw(musicVolText);
    window.draw(musicMuteBtn);
    window.draw(musicMuteText);

    window.draw(soundLabel);
    window.draw(soundBar);
    window.draw(soundCursor);
    window.draw(soundVolText);
    window.draw(soundMuteBtn);
    window.draw(soundMuteText);

    window.draw(returnBtn);
}