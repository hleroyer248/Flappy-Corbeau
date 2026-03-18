#include "OptionMenu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Nécessaire pour sf::Listener

OptionMenu::OptionMenu(const RessourcesManager& rm)
    : background(rm.getSettingsBgTexture()),
    returnBtn(rm.getReturnBtnTexture()),
    infoText(rm.getFont(), "OPTIONS", 80),
    VolumeText(rm.getFont(), "100%", 40), VFXVolumeText(rm.getFont(), "100%", 40) { // Initialisé à 100% par défaut

    // 1. Étirement du background pour une fenêtre 1920x1080 (comme dans ton code)
    float scaleX = 1920.f / rm.getSettingsBgTexture().getSize().x;
    float scaleY = 1080.f / rm.getSettingsBgTexture().getSize().y;
    background.setScale({ scaleX, scaleY });

    // 2. Configuration du Titre
    infoText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = infoText.getLocalBounds();
    infoText.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
        });
    infoText.setPosition({ 960.f, 200.f });

    sliderBar.setSize({ 400.f, 20.f });
    sliderBar.setFillColor(sf::Color(100, 100, 100));
    sliderBar.setPosition({ 760.f, 500.f });
    sliderCursor.setRadius(25.f);
    sliderCursor.setOrigin({ 25.f, 25.f });
    sliderCursor.setFillColor(sf::Color::White);

    // Initialisation de la position du curseur selon le volume actuel du jeu
    volumeValue = sf::Listener::getGlobalVolume();
    float startX = 760.f + (volumeValue / 100.f) * 400.f;
    sliderCursor.setPosition({ startX, 510.f });
    // 5. Texte du Volume (%)
    VolumeText.setFillColor(sf::Color::Green);
    VolumeText.setString(std::to_string(static_cast<int>(volumeValue)) + "%");
    VolumeText.setPosition({ 1200.f, 485.f });

   ///VFX

    vfxSliderBar.setSize({ 400.f, 20.f });
    vfxSliderBar.setFillColor(sf::Color(150, 150, 150));
    vfxSliderBar.setPosition({ 760.f, 600.f }); // en dessous du slider musique

    // Curseur VFX
    vfxSliderCursor.setRadius(25.f);
    vfxSliderCursor.setOrigin({ 25.f, 25.f });
    vfxSliderCursor.setFillColor(sf::Color::White);
    float startVFXX = 760.f + (vfxVolumeValue / 100.f) * 400.f;
    vfxSliderCursor.setPosition({ startVFXX, 610.f });

    // Texte du volume VFX
    VFXVolumeText.setFont(rm.getFont());
    VFXVolumeText.setCharacterSize(40);
    VFXVolumeText.setFillColor(sf::Color::Yellow);
    VFXVolumeText.setString(std::to_string(static_cast<int>(vfxVolumeValue)) + "%");
    VFXVolumeText.setPosition({ 1200.f, 585.f });


        // 6. Bouton Retour
    returnBtn.setPosition({ 700.f, 800.f });
    returnBtn.setScale({ 0.09f, 0.09f });
}

OptionMenu::Action OptionMenu::handleEvent(const sf::Event& event) {

    // --- CLIC APPUYÉ ---
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));

            // On vérifie si on attrape le curseur
            if (sliderCursor.getGlobalBounds().contains(mPos)) {
                isDraggingVolume = true;
            }

            else if (vfxSliderCursor.getGlobalBounds().contains(mPos)) {//isDraggingVFX = true;
                isDraggingVFX = true;
            }
        }
    }

    // --- MOUVEMENT SOURIS ---
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        // if (isDraggingVolume) {
        float mouseX = static_cast<float>(mouseMove->position.x);
        float minX = 760.f;
        float maxX = 1160.f;

        if (isDraggingVolume) {
            // Contrainte du curseur sur la barre
            if (mouseX < minX) mouseX = minX;
            if (mouseX > maxX) mouseX = maxX;

            sliderCursor.setPosition({ mouseX, 510.f });
            //vfxSliderCursor.setPosition({ mouseX, 610.f });

            // Calcul du volume (0 à 100)
            float ratio = (mouseX - minX) / 400.f;
            volumeValue = ratio * 100.f;
           

            // APPLICATION RÉELLE DU VOLUME
            sf::Listener::setGlobalVolume(volumeValue);
 

             // Mise à jour du texte
            VolumeText.setString(std::to_string(static_cast<int>(volumeValue)) + "%");

        }

        if (isDraggingVFX) {
            if (mouseX < minX) mouseX = minX;
            if (mouseX > maxX) mouseX = maxX;

            vfxSliderCursor.setPosition({ mouseX, 610.f });

            float ratio = (mouseX - minX) / 400.f;
            vfxVolumeValue = ratio * 100.f;

            VFXVolumeText.setString(std::to_string(static_cast<int>(vfxVolumeValue)) + "%");
               /*extern AudioManager* globalAM; // ou passe le via OptionMenu
            if (globalAM) globalAM->setVFXVolume(vfxVolumeValue)*/;

            // Ici, applique le volume à ton manager de sons s'il existe
            /* if (globalAM) globalAM->setVFXVolume(vfxVolumeValue); */
        }
    }



    // --- CLIC RELÂCHÉ ---
    if (const auto* mouseRelease = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRelease->button == sf::Mouse::Button::Left) {
            isDraggingVolume = false;

            sf::Vector2f mPos(static_cast<float>(mouseRelease->position.x), static_cast<float>(mouseRelease->position.y));



            // Vérification bouton retour
            if (returnBtn.getGlobalBounds().contains(mPos)) {
                return Action::Return;
            }
        }
    }
    if (const auto* mouseRelease = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRelease->button == sf::Mouse::Button::Left) isDraggingVFX = false;
    }

    return Action::None;
}

float OptionMenu::getVolume() const {
    return volumeValue;
}





void OptionMenu::update(const sf::RenderWindow& window) {}

void OptionMenu::draw(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(infoText);
    window.draw(sliderBar);
    window.draw(vfxSliderBar);
    window.draw(sliderCursor);
    window.draw(vfxSliderCursor);
    window.draw(VolumeText);
    window.draw(VFXVolumeText);
    window.draw(returnBtn);
}