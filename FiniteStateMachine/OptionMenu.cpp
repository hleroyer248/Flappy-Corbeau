#include "OptionMenu.h"
#include <SFML/Graphics.hpp>

OptionMenu::OptionMenu(const RessourcesManager& rm)

//texture
    : background(rm.getSettingsBgTexture()),
    returnBtn(rm.getReturnBtnTexture()),

    // Titre
    infoText(rm.getFont(), "OPTIONS", 60), VolumeText(rm.getFont(), "50%", 30) {
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({ 400.f, 50.f });

    VolumeText.setFillColor(sf::Color::Green);
    VolumeText.setPosition({ 380.f, 290.f }); // À côté de la barre (250+200+70)

    sf::FloatRect textBounds = infoText.getLocalBounds();
    infoText.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
        });  //origine au milieu du texte (horizontalement et verticalement)


    // Slider pour le Volume
    sliderBar.setSize({ 200.f, 10.f });
    sliderBar.setFillColor(sf::Color(100, 100, 100));
    sliderBar.setPosition({ 300.f, 350.f });

    sliderCursor.setRadius(15.f);
    sliderCursor.setOrigin({ 15.f, 15.f }); // Centre le cercle
    sliderCursor.setFillColor(sf::Color::White);
    sliderCursor.setPosition({ 400.f, 355.f });

//bouton de retour
  returnBtn.setPosition({ 600.f, 500.f });
}


OptionMenu::Action OptionMenu::handleEvent(const sf::Event& event) {

    //Action de retour avec le bouton
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse->button == sf::Mouse::Button::Left) {

            sf::Vector2f mPos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));

            if (returnBtn.getGlobalBounds().contains(mPos)) return Action::Return;
        }
    }


    // Réaction du curseur du volume face au clic
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));

            // Si on clique sur le petit cercle blanc
            if (sliderCursor.getGlobalBounds().contains(mPos)) {
                isDraggingVolume = true;
            }
        }
    }


  //Mouvement du curseur avec la souris
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (isDraggingVolume) {

            // Limites de la barre (250.f est ta position X de sliderBar, 200.f est sa largeur)
            float mouseX = static_cast<float>(mouseMove->position.x);
            float minX = 310.f;
            float maxX = 490.f;

            // On contraint la valeur de X entre le début et la fin de la barre
            if (mouseX < minX) mouseX = minX;
            if (mouseX > maxX) mouseX = maxX;

            // On met à jour la position du curseur (Y reste fixe à 355.f)
            sliderCursor.setPosition({ mouseX, 355.f });


            float ratio = (mouseX - minX) / 200.f;
            volumeValue = ratio * 100.f;

            VolumeText.setString(std::to_string(static_cast<int>(volumeValue)) + "%");
        }
    }

    // 3. RELACHEMENT DU CLIC
    if (event.is<sf::Event::MouseButtonReleased>()) {
        isDraggingVolume = false; // On arrête de déplacer peu importe où est la souris

        
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

