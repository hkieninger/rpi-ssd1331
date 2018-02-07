#ifndef SSD1331_H_
#define SSD1331_H_ 1

/*
 * macro to select the type of pin numbering, important for SSD1331::begin() to know the channel associated with the pin
 * WIRINGPI_NUMBERING for wiringPiSetup() -> default for the case none of these are defined (for wiring look at https://de.pinout.xyz/pinout/wiringpi#)
 * GPIO_NUMBERING for wiringPiSetupGpio()
 * PHYS_NUMBERING for wiringPiSetupPhys()
 */
#define WIRINGPI_NUMBERING 1

/*
 * if the macro is set the programm controll wether the passed parameter depass the range
 * to turn off this function only comment out the macro below
 */
#define CHECK_RANGE 1

//c
#include <stdint.h>
//h
#include "display.h"

//width and height of the display controlled by an SSD1331
#define SSD1331_WIDTH 96
#define SSD1331_HEIGHT 64

//SSD1331 maximal clock frequency
#define SSD1331_CLOCK_FREQUENCY 6000000

//time in microseconds to delay after executing drawing command that ssd1331 has time to set the GDDRAM
#define CLEAR_SLEEP_MICROS 500
#define FILL_RECT_SLEEP_MICROS 500
#define DRAW_RECT_SLEEP_MICROS 100
#define DRAW_LINE_SLEEP_MICROS 100

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		0x21
#define SSD1331_CMD_DRAWRECT 		0x22
#define SSD1331_CMD_CLEAR           0x25
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
 * connect the display to the spi interface: https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial
 * you don't need the miso pin, but need additionaly a dc and rst pin
 */
class SSD1331: public Display {
    //pins to use for the communication with the SSD1331 driver
    int cs, dc, rst;
    //file descriptor of the SPI interface
    int fdSPI;
    //channel of wiringPi spi (0 or 1) see: http://wiringpi.com/reference/spi-library/
    int channel;
    public:
        /*
         * constructor
         * @throw: std::invalid_argument
         * @cs: chip select pin: for wiringPi numbering 10 or 11, for phys numbering 24 or 26, for gpio/bcm numbering 8 or 7 
         * @dc: data/command pin, number of the pin for data/command selection, pin numbering according to the called wiringPi setup function
         * @rst: reset pin, number of the pin to reset the display, pin numbering according to the called wiringPi setup function
         */
        SSD1331(int cs, int dc, int rst);

        /*
         * must be called before any draw function, you must call one of the wiringPi setup function in advance
         * resets the display, opens the SPI file descriptor, turns the screen on
         * @throw: GPIOException
         */
        void begin(void);

        /*
         * should be called when your done with the display
         * releases the resources: turns the screen off, closes the SPI file descriptor
         * @throw: GPIOException
         */
        void end(void);

        /*
         * turns the display on
         * @throw: GPIOException
         */
        void turnOn(void);

        /*
         * turns the display off
         * @throw: GPIOException
         */
        void turnOff(void);

        /*
         * puts the pointer of the SSD1331 Graphic Display Data RAM to the specified x and y
         * @throw: GPIOException, std::out_of_range
         * @x: the x coordinate of the pointer
         * @y: the y coordinate of the pointer
         */
        void goTo(uint16_t x, uint16_t y);

        /*
         * writes a color to the Graphic Display Data RAM at the current pointer position
         * and increments the pointer position to the next color (horizontally)
         * @color: the color to push
         */
        void pushColor(uint16_t color);

        /*
         * sends lenght bytes to the display as data
         * @throw: GPIOException
         * @data: pointer to the data, don't forget that raspberry works with little endian while ssd1331 needs big endian
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
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException
         */
        void setRotation(uint8_t rotation) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawPixel(uint16_t x, uint16_t y, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawPoint(uint16_t x, uint16_t y, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void clear(void) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawVerticalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) override;

        /*
         * overrides the method of the Display super class, for specifications look up the header file
         * @throw: GPIOException, std::out_of_range
         */
        void drawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *buffer) override;

};

#endif /* SSD1331_H_ */
