//c
#include <unistd.h>
#include <wiringPi.h>
//h
#include "../Programm/bitmap.h"
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
    //load the picture to display
    Bitmap bmp("raspberry.jpg"); //bitmap should already have the wished dimensions
    //rotate the screen 4 times
    for(int i = 0; i < 4; i++) {
        //set the rotation
        display.setRotation(i);
        //clear the screen
        display.clear();
        //draw the bitmap
        display.drawBitmap(0, 0, bmp);
        //delay 5 seconds
        sleep(5);
    }
    //turn the display off and release the resources
    display.end();
    //exit the programm
    return 0;
}