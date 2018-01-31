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
//hans
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
    if(x >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 goTo, x is out of range");
    if(y >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 goTo, y is out of range");
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

void SSD1331::drawPoint(uint16_t x, uint16_t y, uint16_t color) {
    goTo(x, y);
    pushColor(color);
}