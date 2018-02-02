//c
#include <unistd.h> //sleep()
#include <wiringPi.h> //wiringPiSetupGpio()
//h
#include "display.h" //Display::color565()
#include "ssd1331.h" //SSD1331, .begin(), .drawPoint(), .end()

#define CS 10
#define DC 24
#define RST 25

int main(void) {
    wiringPiSetup();
    SSD1331 display(CS, DC, RST);
    display.begin();
    display.clear();
    display.drawLine(10, 10, 30, 40, D_RED);
    sleep(10);
    display.end();
}