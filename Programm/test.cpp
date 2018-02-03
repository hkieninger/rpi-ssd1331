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
    display.drawEllipse(0, 0, display.getWidth() - 1, display.getHeight() - 1, D_BLUE);
    sleep(5);
    display.fillEllipse(20, 20, display.getWidth() - 21, display.getHeight() - 21, D_GREEN);
    sleep(10);
    display.end();
}