#pragma once
#include <Toggle.h>
#include "../Config/Config.h"

class ButtonManager {
public:
    ButtonManager();
    void begin();
    void poll();
    bool isPressed();
    bool isReleased();
private:
    Toggle button;
}; 