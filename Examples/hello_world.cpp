//c
#include <unistd.h>
#include <wiringPi.h>
//h
#include "../Programm/display.h"
#include "../Programm/ssd1331.h"

/*
 * this example uses the wiringPi pin numbering (for a graphic look at: https://pinout.xyz/pinout/wiringpi#)
 * you don't need to specify the other pins for spi since the programm uses
 * the unique hardware spi interface of the raspberry pi
 */
#define CS 10 //chip select pin
#define DC 24 //data/command pin
#define RST 25 //reset pin

int main(void) {
    //setup wiringPi the underlying library to access the hardware
    wiringPiSetup();
    //create a SSD1331 object wich represents the display with the ssd1331 driver
    SSD1331 display(CS, DC, RST);
    //setup the display and turn it on
    display.begin();
    //draw the Hello World string on the display (for specification of the parameter look up display.h header file)
    display.drawString(0, 0, D_WHITE, "Hello\nWorld\n!!!");
    //delay 20 seconds
    sleep(20);
    //turn the display off and release the resources
    display.end();
    //exit the programm
    return 0;
}