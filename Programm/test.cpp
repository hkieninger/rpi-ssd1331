//c
#include <unistd.h> //sleep()
#include <wiringPi.h> //wiringPiSetupGpio()
//h
#include "bitmap.h"
#include "display.h" //Display::color565()
#include "ssd1331.h" //SSD1331, .begin(), .end()

#define CS 10
#define DC 24
#define RST 25

int main(void) {
    wiringPiSetup();
    SSD1331 display(CS, DC, RST);
    display.begin();
    Bitmap tux("tux.jpg");
    display.drawBitmap(0, 0, tux);
    sleep(3);
    /*display.clear();
    sleep(3);
    display.drawRect(0, 0, 20, 30, D_BLUE);
    sleep(3);
    display.clear();
    Bitmap raspberry("raspberry.jpg");
    display.drawBitmap(20, 0, raspberry);
    sleep(3);
    display.clear();
    display.drawBitmap(50, 10, raspberry);
    sleep(3);*/
    display.end();
    return 0;
}