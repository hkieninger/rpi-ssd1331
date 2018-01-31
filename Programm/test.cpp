//c
#include <unistd.h> //sleep()
#include <wiringPi.h> //wiringPiSetupGpio()
//hans
#include "display.h" //Display::color565()
#include "ssd1331.h" //SSD1331, .begin(), .drawPoint(), .end()

#define CS 10
#define DC 24
#define RST 25

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

int main(void) {
    wiringPiSetup();
    SSD1331 display(CS, DC, RST);
    display.begin();
    for(int x = 0; x < SSD1331_WIDTH; x++) {
        for(int y = 0; y < SSD1331_HEIGHT; y++) {
            display.drawPoint(x, y, 0xF8);
        }
    }
    sleep(10);
    display.end();
}