#include "SlowMotion.h"

SlowMotion::SlowMotion() {
    active = false;
    duration = 3.f;        // durée du slow
    timer = 0.f;

    cooldown = 5.f;        // cooldown
    cooldownTimer = 0.f;

    slowFactor = 0.5f;     // vitesse réduite
}

void SlowMotion::update(float dt) {
    if (active) {
        timer += dt;
        if (timer >= duration) {
            active = false;
            timer = 0.f;
        }
    }
    else {
        if (cooldownTimer < cooldown)
            cooldownTimer += dt;
    }
}

void SlowMotion::activate() {
    if (canActivate()) {
        active = true;
        cooldownTimer = 0.f;
    }
}

bool SlowMotion::isActive() const {
    return active;
}

bool SlowMotion::canActivate() const {
    return cooldownTimer >= cooldown;
}

float SlowMotion::getTimeScale() const {
    return active ? slowFactor : 1.f;
}