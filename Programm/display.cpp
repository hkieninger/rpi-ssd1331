//c++
#include <string>
//c
#include <stdint.h> //uint16_t, uint8_t, int8_t, int32_t, int64_t, uint64_t
#include <stdlib.h> //abs()
//h
#include "bitmap.h"
#include "alpha_bitmap.h"
#include "character.h"
#include "display.h"

Display::Display(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
    rotation = 0;
}

uint16_t Display::getWidth(void) {
    return width;
}

uint16_t Display::getHeight(void) {
    return height;
}

void Display::setRotation(uint8_t rotation) {
    rotation %= 4;
    if((this->rotation - rotation) % 2 != 0) {
        uint16_t temp = width;
        width = height;
        height = temp;
    }
    this->rotation = rotation;
}

uint8_t Display::getRotation(void) {
    return rotation;
}

void Display::drawPoint(uint16_t x, uint16_t y, uint16_t color) {
    uint16_t temp;
    switch(rotation) {
        case 1:
            temp = width - x - 1;
            x = y;
            y = temp;
            break;
        case 2:
            x = width - x - 1;
            y = height - y - 1;
            break;
        case 3:
            temp = x;
            x = height - y - 1;
            y = temp;
    }
    drawPixel(x, y, color);
}

void Display::clear(void) {
    for(uint16_t x = 0; x < width; x++) {
        for(uint16_t y = 0; y < height; y++) {
            drawPoint(x, y, 0);
        }
    }
}

void Display::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
    for(uint16_t i = 0; i < len; i++) {
        drawPoint(x + i, y, color);
    }
}

void Display::drawVerticalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
    for(uint16_t i = 0; i < len; i++) {
        drawPoint(x, y + i, color);
    }
}

//for a better implementation use bresenhams-algorithm https://de.wikipedia.org/wiki/Bresenham-Algorithmus
void Display::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint16_t dx = abs(x1 - x0) + 1;
    uint16_t dy = abs(y1 - y0) + 1;
    if(dx > dy) {
        if(x0 > x1) {
            //swap points
            uint16_t temp;
            temp = x0;
            x0 = x1;
            x1 = temp;
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
        uint16_t slope = dx / dy; //amount of pixels in x direction per pixel in y direction
        uint16_t remain = dx % dy; //remain of pixels in x direction per pixel in y direction
        uint16_t remainDist = 65535; //distribute the remaining pixel more or less equally to avoid bent in the line
        uint16_t remainDistRemain = 0; //remaining of the distribution
        if(remain > 0) {
            remainDist = dy / remain; 
            remainDistRemain = dy % remain;
        }
        int8_t direction = (y0 < y1) ? 1 : -1; //up or down
        //draw the line through composition of several horizontal lines
        while(x0 <= x1) {
            uint16_t len = slope;
            if(y0 % remainDist == 0) {
                len++;
            } else if(remainDistRemain > 0) {
                len++;
                remainDistRemain--;
            }
            drawHorizontalLine(x0, y0, len, color);
            x0 += len;
            y0 += direction;
        }
    } else {
        if(y0 > y1) {
            //swap points
            uint16_t temp;
            temp = x0;
            x0 = x1;
            x1 = temp;
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
        uint16_t slope = dy / dx; //amount of pixels in y direction per pixel in x direction
        uint16_t remain = dy % dx; //remain of pixels in y direction per pixel in x direction
        uint16_t remainDist = 65535; //distribute the remaining pixel more or less equally to avoid bent in the line
        uint16_t remainDistRemain = 0; //remaining of the distribution
        if(remain > 0) {
            remainDist = dx / remain; 
            remainDistRemain = dx % remain;
        }
        int8_t direction = (x0 < x1) ? 1 : -1; //left or right
        //draw the line through composition of several vertical lines
        while(y0 <= y1) {
            uint16_t len = slope;
            if(x0 % remainDist == 0) {
                len++;
            } else if(remainDistRemain > 0) {
                len++;
                remainDistRemain--;
            }
            drawVerticalLine(x0, y0, len, color);
            y0 += len;
            x0 += direction;
        }
    }
}

void Display::drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    if(x0 > x1) {
        uint16_t temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if(y0 > y1) {
        uint16_t temp = y0;
        y0 = y1;
        y1 = temp;
    }
    drawHorizontalLine(x0, y0, x1 - x0 + 1, color);
    drawHorizontalLine(x0, y1, x1 - x0 + 1, color);
    drawVerticalLine(x0, y0 + 1, y1 - y0 - 1, color);
    drawVerticalLine(x1, y0 + 1, y1 - y0 - 1, color);
}

void Display::fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    if(x0 > x1) {
        uint16_t temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if(y0 > y1) {
        uint16_t temp = y0;
        y0 = y1;
        y1 = temp;
    }
    uint16_t len = y1 - y0 + 1;
    for(uint16_t x = x0; x <= x1; x++) {
        drawVerticalLine(x, y0, len, color);
    }
}

//code copied and adapted from https://de.wikipedia.org/wiki/Bresenham-Algorithmus
void Display::drawEllipse(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint16_t a = abs(x1 - x0) / 2;
    uint16_t b = abs(y1 - y0) / 2;
    uint16_t x = a + ((x0 < x1) ? x0 : x1);
    uint16_t y = b + ((y0 < y1) ? y0 : y1);
    int32_t dx = 0, dy = b; /* im I. Quadranten von links oben nach rechts unten */
    int64_t a2 = a * a, b2 = b * b;
    int64_t err = b2 - (2 * b - 1) * a2, e2; /* Fehler im 1. Schritt */

    do {
        drawPoint(x + dx, y + dy, color); /* I. Quadrant */
        drawPoint(x - dx, y + dy, color); /* II. Quadrant */
        drawPoint(x - dx, y - dy, color); /* III. Quadrant */
        drawPoint(x + dx, y - dy, color); /* IV. Quadrant */

        e2 = 2 * err;
        if(e2 < (2 * dx + 1) * b2) { 
            dx++;
            err += (2 * dx + 1) * b2; 
        }
        if(e2 > -(2 * dy - 1) * a2) {
            dy--;
            err -= (2 * dy - 1) * a2;
        }
    } while (dy >= 0);

    while (dx++ < a) { /* fehlerhafter Abbruch bei flachen Ellipsen (b=1) */
        drawPoint(x + dx, y, color); /* -> Spitze der Ellipse vollenden */
        drawPoint(x - dx, y, color);
    }
}

//code copied and adapted from https://de.wikipedia.org/wiki/Bresenham-Algorithmus
void Display::fillEllipse(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint16_t a = abs(x1 - x0) / 2;
    uint16_t b = abs(y1 - y0) / 2;
    uint16_t x = a + ((x0 < x1) ? x0 : x1);
    uint16_t y = b + ((y0 < y1) ? y0 : y1);
    int32_t dx = 0, dy = b; /* im I. Quadranten von links oben nach rechts unten */
    int64_t a2 = a * a, b2 = b * b;
    int64_t err = b2 - (2 * b - 1) * a2, e2; /* Fehler im 1. Schritt */

    do {
        e2 = 2 * err;
        if(e2 < (2 * dx + 1) * b2) { 
            dx++;
            err += (2 * dx + 1) * b2; 
        }
        if(e2 > -(2 * dy - 1) * a2) {
            drawHorizontalLine(x, y + dy, dx + 1, color); /* I. Quadrant */
            drawHorizontalLine(x - dx, y + dy, dx, color); /* II. Quadrant */
            drawHorizontalLine(x - dx, y - dy, dx, color); /* III. Quadrant */
            drawHorizontalLine(x, y - dy, dx + 1, color); /* IV. Quadrant */
            dy--;
            err -= (2 * dy - 1) * a2;
        }
    } while (dy >= 0);

    while (dx++ < a) { /* fehlerhafter Abbruch bei flachen Ellipsen (b=1) */
        drawPoint(x + dx, y, color); /* -> Spitze der Ellipse vollenden */
        drawPoint(x - dx, y, color);
    }
}

void Display::drawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *buffer) {
    for(int w = 0; w < width; w++) {
        for(int h = 0; h < height; h++) {
            drawPoint(x + w, y + h, buffer[h * width + w]);
        }
    }
}

void Display::drawAlphaBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *buffer, bool *alpha) {
    for(int w = 0; w < width; w++) {
        for(int h = 0; h < height; h++) {
            if(alpha[h * width + w])
                drawPoint(x + w, y + h, buffer[h * width + w]);
        }
    }
}

void Display::drawBitmap(uint16_t x, uint16_t y, Bitmap& bitmap) {
    drawBuffer(x, y, bitmap.getWidth(), bitmap.getHeight(), bitmap.getBuffer());
}

void Display::drawAlphaBitmap(uint16_t x, uint16_t y, AlphaBitmap& bitmap) {
    drawAlphaBuffer(x, y, bitmap.getWidth(), bitmap.getHeight(), bitmap.getBuffer(), bitmap.getAlpha());
}

void Display::drawChar(uint16_t x, uint16_t y, uint16_t color, char c) {
    uint64_t character = UNKNOWN_CHARACTER;
    if(c >= 'A' && c <= 'Z') {
        character = LETTER_CHARACTER[c - 'A'];
    } else if(c >= 'a' && c <= 'z') {
        character = LETTER_CHARACTER[c - 'a'];
    } else if(c >= '0' && c <= '9') {
        character = DIGIT_CHARACTER[c - '0'];
    } else if(c == ' ') {
        character = PUNCTUATION_CHARACTER[0];
    } else if(c == '!') {
        character = PUNCTUATION_CHARACTER[1];
    } else if (c == '?') {
        character = PUNCTUATION_CHARACTER[2];
    } else if(c >= '<' && c <= '>') {
        character = OPERATOR_CHARACTER[c - '<'];
    } else if(c == '+') {
        character = OPERATOR_CHARACTER[3];
    } else if(c == '-') {
        character = OPERATOR_CHARACTER[4];
    } else if(c == '*') {
        character = OPERATOR_CHARACTER[5];
    } else if(c == '/') {
        character = OPERATOR_CHARACTER[6];
    }

    uint64_t mask  = 1;
    for(uint16_t h = 0; h < CHARACTER_HEIGHT; h++) {
        for(uint16_t w = 0; w < CHARACTER_WIDTH; w++) {
            if(mask & character) {
                drawPoint(x + CHARACTER_WIDTH - 1 - w, y + CHARACTER_HEIGHT - 1 - h, color);
            }
            mask <<= 1;
        }
    }
}

void Display::drawString(uint16_t x, uint16_t y, uint16_t color, std::string str) {
    uint16_t startX = x;
    for(uint32_t i = 0; i < str.length(); i++) {
        if(str[i] == '\n') {
            y += CHARACTER_HEIGHT + 1 + CHARACTER_HEIGHT / 5;
            x = startX;
        } else {
            drawChar(x, y, color, str[i]);
            x += CHARACTER_WIDTH + 1 + CHARACTER_WIDTH / 10;
        }
    }
}

uint16_t Display::color565(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t color = red >> 3;
    color <<= 6;
    color |= green >> 2;
    color <<= 5;
    color |= blue >> 3;
    return color;
}