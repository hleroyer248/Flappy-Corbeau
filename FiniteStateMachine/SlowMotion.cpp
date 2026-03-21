#include "SlowMotion.h"

SlowMotion::SlowMotion() {
    active = false;
    duration = 3.f;        // durée du slow
    timer = 0.f;

    cooldown = 5.f;        // cooldown
    cooldownTimer = 0.f;

    slowFactor = 0.5f;  
    hasBeenUsed = false;
}

void SlowMotion::reset() {
    active = false;
    timer = 0.f;
    cooldownTimer = 0.f;
    hasBeenUsed = false;
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
        if (hasBeenUsed && cooldownTimer < cooldown)
            cooldownTimer += dt;
    }
}

void SlowMotion::activate() {
    if (canActivate()) {
        active = true;
        cooldownTimer = 0.f;
        hasBeenUsed = true; 
    }
}

bool SlowMotion::isActive() const {
    return active;
}

bool SlowMotion::canActivate() const {
    return !hasBeenUsed || cooldownTimer >= cooldown;
}

float SlowMotion::getTimeScale() const {
    return active ? slowFactor : 1.f;
}

float SlowMotion::getRemainingTime() const {
    return duration - timer;
}

float SlowMotion::getCooldownRatio() const {
    if (cooldown <= 0.f) return 0.f;
    return cooldownTimer / cooldown;
}

bool SlowMotion::isOnCooldown() const {
    return hasBeenUsed && cooldownTimer > 0.f && cooldownTimer < cooldown && !active;
}