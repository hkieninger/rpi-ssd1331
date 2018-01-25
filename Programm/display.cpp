//c
#include <stdint.h> //uint16_t, uint8_t
//hans
#include "display.h"

Display::Display(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
}

uint16_t Display::color565(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t color = red >> 3;
    color <<= 6;
    color |= green >> 2;
    color <<= 5;
    color |= blue >> 3;
    return color;
}