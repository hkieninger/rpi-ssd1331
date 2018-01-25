//c++
#include <string> //std::string
#include <stdexcept> //std::out_of_range
//c
#include <stdint.h> //uint16_t, uint8_t
#include <string.h> //strerror()
#include <errno.h> //errno
#include <unistd.h> //write(), close()
#include <wiringPi.h> //pinMode(), digitalWrite()
#include <wiringPiSPI.h> //wiringPiSPISetup()
//hans
#include "gpio_exception.h" //GPIOException
#include "ssd1331.h"

SSD1331::SSD1331(int cs, int dc, int rst) : Display(SSD1331_WIDTH, SSD1331_HEIGHT) {
    this->cs = cs;
    this->dc = dc;
    this->rst = rst;
    fdSPI = -1;
}

void SSD1331::begin(void) {
    //declare the pins as output
    pinMode(cs, OUTPUT);
    pinMode(dc, OUTPUT);
    pinMode(rst, OUTPUT);
    //reset the display
    digitalWrite(cs, LOW);
    digitalWrite(rst, HIGH);
    delay(500); //TODO: reduce
    digitalWrite(rst, LOW);
    delay(500); //TODO: reduce
    digitalWrite(rst, HIGH);
    delay(500); //TODO: reduce
    //open the spi file descriptor
    int channel; //TODO: proper handling
    if(cs == 10)
        channel = 0;
    else
        channel = 1;
    fdSPI = wiringPiSPISetup(channel, SSD1331_CLOCK_FREQUENCY);
    if(fdSPI == -1)
        throw GPIOException("SSD1331 begin, wiringPi SPI setup: " + std::string(strerror(errno)));
    //initialisation sequence
    writeCommand(SSD1331_CMD_DISPLAYOFF);  	    // 0xAE
    writeCommand(SSD1331_CMD_SETREMAP); 	    // 0xA0
    writeCommand(0x72);		                    // 0x72 for RGB or 0x76 for BGR
    writeCommand(SSD1331_CMD_STARTLINE); 	    // 0xA1
    writeCommand(0x0);
    writeCommand(SSD1331_CMD_DISPLAYOFFSET);    // 0xA2
    writeCommand(0x0);
    writeCommand(SSD1331_CMD_NORMALDISPLAY);  	// 0xA4
    writeCommand(SSD1331_CMD_SETMULTIPLEX); 	// 0xA8
    writeCommand(0x3F);  			            // 0x3F 1/64 duty
    writeCommand(SSD1331_CMD_SETMASTER);  	    // 0xAD
    writeCommand(0x8E);
    writeCommand(SSD1331_CMD_POWERMODE);  	    // 0xB0
    writeCommand(0x0B);
    writeCommand(SSD1331_CMD_PRECHARGE);  	    // 0xB1
    writeCommand(0x31);
    writeCommand(SSD1331_CMD_CLOCKDIV);  	    // 0xB3
    writeCommand(0xF0);                         // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    writeCommand(SSD1331_CMD_PRECHARGEA);  	    // 0x8A
    writeCommand(0x64);
    writeCommand(SSD1331_CMD_PRECHARGEB);  	    // 0x8B
    writeCommand(0x78);
    writeCommand(SSD1331_CMD_PRECHARGEC);  	    // 0x8C
    writeCommand(0x64);
    writeCommand(SSD1331_CMD_PRECHARGELEVEL);  	// 0xBB
    writeCommand(0x3A);
    writeCommand(SSD1331_CMD_VCOMH);  		    // 0xBE
    writeCommand(0x3E);
    writeCommand(SSD1331_CMD_MASTERCURRENT);  	// 0x87
    writeCommand(0x06);
    writeCommand(SSD1331_CMD_CONTRASTA);  	    // 0x81
    writeCommand(0x91);
    writeCommand(SSD1331_CMD_CONTRASTB);  	    // 0x82
    writeCommand(0x50);
    writeCommand(SSD1331_CMD_CONTRASTC);  	    // 0x83
    writeCommand(0x7D);
    writeCommand(SSD1331_CMD_DISPLAYON);	    // 0xAF   
}

void SSD1331::end(void) {
    //close spi file descriptor
    if(close(fdSPI) == -1)
        throw GPIOException("SSD1331 end, close SPI file descriptor: " + std::string(strerror(errno)));
}

void SSD1331::goTo(uint16_t x, uint16_t y) {
    if(x >= SSD1331_WIDTH)
        throw std::out_of_range("SSD1331 goTo, x is out of range");
    if(y >= SSD1331_HEIGHT)
        throw std::out_of_range("SSD1331 goTo, y is out of range");
    // set x and y coordinate
    writeCommand(SSD1331_CMD_SETCOLUMN);
    writeCommand(x);
    writeCommand(SSD1331_WIDTH - 1);

    writeCommand(SSD1331_CMD_SETROW);
    writeCommand(y);
    writeCommand(SSD1331_HEIGHT - 1);
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
    writeData((uint8_t * ) &color, 2);
}