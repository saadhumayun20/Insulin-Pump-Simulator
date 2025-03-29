#include "Cartridge.h"

Cartridge::Cartridge(float capacity):capacity(capacity), currentLevel(capacity) {}

void Cartridge::refill() {
    currentLevel = capacity;
}

float Cartridge::getCurrentLevel() const { return currentLevel; }
