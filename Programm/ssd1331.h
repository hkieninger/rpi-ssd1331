#ifndef SSD1331_H_
#define SSD1331_H_ 1

//c
#include <stdint.h>
//hans
#include "display.h"

//width and height of the display controlled by an SSD1331
#define SSD1331_WIDTH 96
#define SSD1331_HEIGHT 64

//SSD1331 maximal clock frequency
#define SSD1331_CLOCK_FREQUENCY 6000000

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		0x21
#define SSD1331_CMD_DRAWRECT 		0x22
#define SSD1331_CMD_FILL 			0x26
#define SSD1331_CMD_SETCOLUMN 		0x15
#define SSD1331_CMD_SETROW    		0x75
#define SSD1331_CMD_CONTRASTA 		0x81
#define SSD1331_CMD_CONTRASTB 		0x82
#define SSD1331_CMD_CONTRASTC		0x83
#define SSD1331_CMD_MASTERCURRENT 	0x87
#define SSD1331_CMD_SETREMAP 		0xA0
#define SSD1331_CMD_STARTLINE 		0xA1
#define SSD1331_CMD_DISPLAYOFFSET 	0xA2
#define SSD1331_CMD_NORMALDISPLAY 	0xA4
#define SSD1331_CMD_DISPLAYALLON  	0xA5
#define SSD1331_CMD_DISPLAYALLOFF 	0xA6
#define SSD1331_CMD_INVERTDISPLAY 	0xA7
#define SSD1331_CMD_SETMULTIPLEX  	0xA8
#define SSD1331_CMD_SETMASTER 		0xAD
#define SSD1331_CMD_DISPLAYOFF 		0xAE
#define SSD1331_CMD_DISPLAYON     	0xAF
#define SSD1331_CMD_POWERMODE 		0xB0
#define SSD1331_CMD_PRECHARGE 		0xB1
#define SSD1331_CMD_CLOCKDIV 		0xB3
#define SSD1331_CMD_PRECHARGEA 		0x8A
#define SSD1331_CMD_PRECHARGEB 		0x8B
#define SSD1331_CMD_PRECHARGEC 		0x8C
#define SSD1331_CMD_PRECHARGELEVEL 	0xBB
#define SSD1331_CMD_VCOMH 			0xBE

/* 
 * represents a display wich works with the SSD1331 driver
 * datasheet: https://cdn-shop.adafruit.com/datasheets/SSD1331_1.2.pdf
 */
class SSD1331: public Display {
    //pins to use for the communication with the SSD1331 driver
    int cs, dc, rst;
    //file descriptor of the SPI interface
    int fdSPI;
    public:
        /*
         * constructor
         * @throw: GPIOException
         * @cs: chip select pin, channel of wiringPi spi (0 or 1) see: https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial
         * @dc: data/command pin, number of the pin for data/command selection, pin numbering according to the called wiringPi setup function
         * @rst: reset pin, number of the pin to reset the display, pin numbering according to the called wiringPi setup function
         */
        SSD1331(int cs, int dc, int rst);

        /*
         * initialises the display, must be called before any draw function
         * you must call one of the wiringPi setup function in advance
         * opens the SPI file descriptor
         */
        void begin(void);

        /*
         * releases the resources
         * closes the SPI file descriptor
         * @throw: GPIOException
         */
        void end(void);

        /*
         * puts the pointer of the SSD1331 RAM to the specified x and y
         * @throw: GPIOException, std::out_of_range
         * @x: the x coordinate of the pointer
         * @y: the y coordinate of the pointer
         */
        void goTo(uint16_t x, uint16_t y);

        /*
         * sends lenght bytes to the display as data
         * @throw: GPIOException
         * @data: pointer to the data
         * @length: the amount of data bytes/uint8_t
         */
        void writeData(uint8_t *data, int length);

        /*
         * sends a command to the display
         * @throw: GPIOException
         * @command: the command
         */
        void writeCommand(uint8_t command);

        /* 
         * overrides the drawPoint function of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawPoint(uint16_t x, uint16_t y, uint16_t color) override;
};

#endif /* SSD1331_H_ */