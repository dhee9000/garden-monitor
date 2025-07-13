#include "ButtonManager.h"

ButtonManager::ButtonManager() : button(BUTTON_PIN) {}

void ButtonManager::begin() {
    button.blink(10);
}

void ButtonManager::poll() {
    button.poll();
}

bool ButtonManager::isPressed() {
    return button.isPressed();
}

bool ButtonManager::isReleased() {
    return button.isReleased();
} 