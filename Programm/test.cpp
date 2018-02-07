//c
#include <unistd.h> //sleep()
#include <wiringPi.h> //wiringPiSetupGpio()
//h
#include "alpha_bitmap.h"
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
    AlphaBitmap gnu("gnu.png");
    for(int i = 0; i < 4; i++) {
        display.clear();
        display.setRotation(i);
        display.drawAlphaBitmap(0, 0, gnu);
        sleep(5);
    }
    display.end();
    return 0;
}