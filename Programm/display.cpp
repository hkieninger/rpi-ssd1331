//c
#include <stdint.h> //uint16_t, uint8_t, int8_t
#include <stdlib.h> //abs()
//h
#include "display.h"

Display::Display(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
}

uint16_t Display::getWidth(void) {
    return width;
}

uint16_t Display::getHeight(void) {
    return height;
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


uint16_t Display::color565(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t color = red >> 3;
    color <<= 6;
    color |= green >> 2;
    color <<= 5;
    color |= blue >> 3;
    return color;
}