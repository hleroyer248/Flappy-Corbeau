#pragma once

class SlowMotion {
private:
    bool active;
    float duration;
    float timer;
    float cooldown;
    float cooldownTimer;
    float slowFactor;
    bool hasBeenUsed;

public:
    SlowMotion();

    void update(float dt);
    void activate();

    bool isActive() const;
    bool canActivate() const;

    float getTimeScale() const;

    float getRemainingTime() const;
    float getCooldownRatio() const;
    bool isOnCooldown() const;
    void reset();
};