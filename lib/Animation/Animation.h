#pragma once

#include <Array.h>

const unsigned int TARGET_DELTA_TIME = 33;

class Animation {
public:
    virtual void update(unsigned long time) = 0;
};

template <size_t MAX_SIZE>
class AnimationManager {
    Array<Animation*, MAX_SIZE> animations;
    size_t currentAnimation = -1;
    unsigned long startTime = 0;

public:
    ~AnimationManager() {
        for (int i = 0; i < animations.size(); i++) {
            delete animations[i];
        }
    }

    void addAnimation(Animation* animation) {
        animations.push_back(animation);
    }

    void activate(size_t index) {
        if (index < animations.size()) {
            currentAnimation = index;
            startTime = millis();
        }
    }

    void update() {
        static unsigned long lastTime = 0;
        delay(TARGET_DELTA_TIME - (millis() - lastTime));
        if (currentAnimation < animations.size() && currentAnimation != -1) {
            animations[currentAnimation]->update(millis() - startTime);
        }
        lastTime = millis();
    }
};
