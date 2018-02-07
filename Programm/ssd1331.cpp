//c++
#include <string> //std::string
#include <stdexcept> //std::out_of_range, std::invalid_argument
//c
#include <stdint.h> //uint16_t, uint8_t
#include <string.h> //strerror()
#include <errno.h> //errno
#include <unistd.h> //write(), close()
#include <wiringPi.h> //pinMode(), digitalWrite()
#include <wiringPiSPI.h> //wiringPiSPISetup()
#include <byteswap.h> //bswap_16()
//h
#include "gpio_exception.h" //GPIOException
#include "ssd1331.h"

SSD1331::SSD1331(int cs, int dc, int rst) : Display(SSD1331_WIDTH, SSD1331_HEIGHT) {
    this->cs = cs;
    this->dc = dc;
    this->rst = rst;
    fdSPI = -1;
    #if defined GPIO_NUMBERING
    switch(cs) {
        case 8: channel = 0; break;
        case 7: channel = 1; break;
        default: throw std::invalid_argument("invalid cs pin, valid cs pins are 8 (bcm) and 7 (bcm)");
    }
    #elif defined PHYS_NUMBERING
    switch(cs) {
        case 24: channel = 0; break;
        case 26: channel = 1; break;
        default: throw std::invalid_argument("invalid cs pin, valid cs pins are 24 (phys) and 26 (phys)");
    }
    #else
    switch(cs) {
        case 10: channel = 0; break;
        case 11: channel = 1; break;
        default: throw std::invalid_argument("invalid cs pin, valid cs pins are 10 (wiringPi) and 11 (wiringPi)");
    }
    #endif
}

void SSD1331::begin(void) {
    //declare the pins as output
    pinMode(cs, OUTPUT);
    pinMode(dc, OUTPUT);
    pinMode(rst, OUTPUT);
    //reset the display
    digitalWrite(cs, LOW);
    digitalWrite(rst, HIGH);
    usleep(3);
    digitalWrite(rst, LOW);
    usleep(3);
    digitalWrite(rst, HIGH);
    usleep(3);
    //open the spi file descriptor
    fdSPI = wiringPiSPISetup(channel, SSD1331_CLOCK_FREQUENCY);
    if(fdSPI == -1)
        throw GPIOException("SSD1331 begin, wiringPi SPI setup (have you enabled spi?): " + std::string(strerror(errno)));
    //turn display off
    turnOff();
    //settings for mapping in the GDDRAM
    writeCommand(SSD1331_CMD_SETREMAP);        // 0xA0
    writeCommand(0x60);
    //clear display
    clear();
    //turn display on
    turnOn();
}

void SSD1331::end(void) {
    //turn display off
    turnOff();
    //close spi file descriptor
    if(close(fdSPI) == -1)
        throw GPIOException("SSD1331 end, close SPI file descriptor: " + std::string(strerror(errno)));
}

void SSD1331::turnOn(void) {
    writeCommand(SSD1331_CMD_DISPLAYON);        //0xAF
}

void SSD1331::turnOff(void) {
    writeCommand(SSD1331_CMD_DISPLAYOFF);       //0xAE
}

void SSD1331::goTo(uint16_t x, uint16_t y) {
    #ifdef CHECK_RANGE
    if(x >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 goTo, x is out of range");
    if(y >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 goTo, y is out of range");
    #endif
    // set x and y coordinate
    writeCommand(SSD1331_CMD_SETCOLUMN);        // 0x15
    writeCommand(x);
    writeCommand(SSD1331_WIDTH - 1);

    writeCommand(SSD1331_CMD_SETROW);           // 0x75
    writeCommand(y);
    writeCommand(SSD1331_HEIGHT - 1);
}

void SSD1331::pushColor(uint16_t color) {
    color = bswap_16(color);
    writeData((uint8_t * ) &color, 2);
}

void SSD1331::writeData(uint8_t *data, int length) {
    //set to data
    digitalWrite(dc, HIGH);
    //write spi
    int written = write(fdSPI, data, length);
    if(written != length) {
        if(written == -1)
            throw GPIOException("SSD1331 writeData, write SPI: " + std::string(strerror(errno)));
        throw GPIOException("SSD1331 writeData, SPI wrote less bytes than specified");
    }
}

void SSD1331::writeCommand(uint8_t command) {
    //set to command
    digitalWrite(dc, LOW);
    //write spi
    int written = write(fdSPI, &command, 1);
    if(written != 1) {
        if(written == -1)
            throw GPIOException("SSD1331 writeCommand, write SPI: " + std::string(strerror(errno)));
        throw GPIOException("SSD1331 writeCommand, didn't write command");
    }
}

void SSD1331::setRotation(uint8_t rotation) {
    Display::setRotation(rotation);
    writeCommand(SSD1331_CMD_SETREMAP);     //0xA0
    switch(this->rotation) {
        case 1: writeCommand(0x70); break;
        case 2: writeCommand(0x72); break;
        case 3: writeCommand(0x62); break;
        default: writeCommand(0x60);
    }
}

void SSD1331::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    goTo(x, y);
    pushColor(color);
}

void SSD1331::drawPoint(uint16_t x, uint16_t y, uint16_t color) {
    if(rotation % 2 == 1) {
        uint16_t temp = x;
        x = y;
        y = temp;
    }
    drawPixel(x, y, color);
}

void SSD1331::clear(void) {
    writeCommand(SSD1331_CMD_CLEAR);        //0x25
    writeCommand(0);
    writeCommand(0);
    writeCommand(SSD1331_WIDTH - 1);
    writeCommand(SSD1331_HEIGHT - 1);
    usleep(CLEAR_SLEEP_MICROS);
}

void SSD1331::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
    if(len > 0)
        drawLine(x, y, x + len - 1, y, color);
}

void SSD1331::drawVerticalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
    if(len > 0)
        drawLine(x, y, x, y + len - 1, color);
}

void SSD1331::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    if(rotation % 2 == 1) {
        uint16_t temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    #ifdef CHECK_RANGE
    if(x0 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 drawLine, x0 is out of range");
    if(y0 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 drawLine, y0 is out of range");
    if(x1 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 drawLine, x1 is out of range");
    if(y1 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 drawLine, y1 is out of range");
    #endif
    //line coordinates
    writeCommand(SSD1331_CMD_DRAWLINE);     //0x21
    writeCommand(x0);
    writeCommand(y0);
    writeCommand(x1);
    writeCommand(y1);
    //line color
    writeCommand(color >> 10 & 0x3E);
    writeCommand(color >> 5 & 0x3F);
    writeCommand(color << 1 & 0x3E);
    usleep(DRAW_LINE_SLEEP_MICROS);
}

void SSD1331::drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    if(rotation % 2 == 1) {
        uint16_t temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    #ifdef CHECK_RANGE
    if(x0 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 drawRect, x0 is out of range");
    if(y0 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 drawRect, y0 is out of range");
    if(x1 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 drawRect, x1 is out of range");
    if(y1 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 drawRect, y1 is out of range");
    #endif
    //turn fill off
    writeCommand(SSD1331_CMD_FILL);         //0x26
    writeCommand(0);
    //rectangle coordinates
    writeCommand(SSD1331_CMD_DRAWRECT);     //0x22
    writeCommand(x0);
    writeCommand(y0);
    writeCommand(x1);
    writeCommand(y1);
    //outline color
    writeCommand(color >> 10 & 0x3E);
    writeCommand(color >> 5 & 0x3F);
    writeCommand(color << 1 & 0x3E);
    //fill color (doesn't mater)
    writeCommand(0);
    writeCommand(0);
    writeCommand(0);
    usleep(DRAW_RECT_SLEEP_MICROS);
}

void SSD1331::fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    if(rotation % 2 == 1) {
        uint16_t temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    #ifdef CHECK_RANGE
    if(x0 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 fillRect, x0 is out of range");
    if(y0 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 fillRect, y0 is out of range");
    if(x1 >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 fillRect, x1 is out of range");
    if(y1 >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 fillRect, y1 is out of range");
    #endif
    //turn fill on
    writeCommand(SSD1331_CMD_FILL);         //0x26
    writeCommand(1);
    //rectangle coordinates
    writeCommand(SSD1331_CMD_DRAWRECT);     //0x22
    writeCommand(x0);
    writeCommand(y0);
    writeCommand(x1);
    writeCommand(y1);
    //outline color
    writeCommand(color >> 10 & 0x3E);
    writeCommand(color >> 5 & 0x3F);
    writeCommand(color << 1 & 0x3E);
    //fill color
    writeCommand(color >> 10 & 0x3E);
    writeCommand(color >> 5 & 0x3F);
    writeCommand(color << 1 & 0x3E);
    usleep(FILL_RECT_SLEEP_MICROS);
}

void SSD1331::drawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *buffer) {
    if(rotation % 2 == 1) {
        uint16_t temp = x;
        x = y;
        y = temp;
    }
    #ifdef CHECK_RANGE
    if(x + width > SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 drawBuffer, x is out of range");
    if(y + height > SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 drawBuffer, y is out of range");
    #endif
    //set boundarys of bitmap
    writeCommand(SSD1331_CMD_SETCOLUMN);        // 0x15
    writeCommand(x);
    writeCommand(x + SSD1331_WIDTH - 1);

    writeCommand(SSD1331_CMD_SETROW);           // 0x75
    writeCommand(y);
    writeCommand(y + SSD1331_HEIGHT - 1);
    //write bitmap
    for(int i = 0; i < width * height; i++) {
        pushColor(buffer[i]);
    }
}